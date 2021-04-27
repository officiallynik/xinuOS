#include <xinu.h>

/**
 * Adds a request edge to the RAG from pid to lockid
 *
 * @param int	pid
 * @param int 	lockid
 */
void rag_request(int pid, int lockid)
{
	//from pid to lockid
	rag[NLOCK + pid][lockid] = 1;
}

/**
 * Adds an allocation edge to the RAG from lockid to pid.
 * Removes request edge from pid to lockid
 * 
 * @param int	pid
 * @param int	lockid
 */
void rag_alloc(int pid, int lockid) 
{
	//adds edge from lockid->pid
	rag[lockid][NLOCK + pid] = 1;
	
	//remove edge from pid->lockid
	rag[NLOCK + pid][lockid] = 0;
}

/**
 * Removes the request or allocation edge from lockid to pid
 *
 * @param int	pid
 * @param int	lockid
 */
void rag_dealloc(int pid, int lockid)
{
	//remove request edge from lockid -> pid
	rag[lockid][NLOCK + pid] = 0;
}

/**
 * Checks the RAG for cycle(s).  
 * For each cycle found, print out DEADLOCK followed by the nodes
 * involved in the cycle.
 * If no cycles are found, then the system is deadlock-free.
 * You should produce no output in this case. 
 * Application of DFS
 *
 * @param void
 */
void deadlock_detect(void)
{
	int n = NPROC + NLOCK;		//holds number of vertices
	int stack[n];			//vertices currently on stack
	int visited[n];			//visited vertices
	int i, j = 0;

	//set int arrays
	for(i = 0; i < n; i++)
	{
		stack[i] = 0;
		visited[i] = 0;
	}

	//call the helper
	for(i = 0; i < n; i++)
	{
		if(isCycle(i, visited, stack) == 1)
		{
			return;
		}
	}				
}

/**
 * Print the cycle given its backedge
 *
 * @param int	vertex_index
 * @param int	initial
 * @param int 	*visited
 * @return int	success or failure
 */
int printAdjacent(int vertex_index, int initial, int *visited)
{
	int i;
	int n = NPROC + NLOCK;
	int adjacent[n];
	int adjacent_size = 0;

	//fill adjacent with all adjacent nodes for a given vertex 
	for(i = 0; i < n; i++)
	{
		//if there is an edge
		if(rag[vertex_index][i] == 1)
		{
			adjacent[adjacent_size] = i;
			adjacent_size++;
		}
	}
	
	if(visited[vertex_index] == 0)
	{

		//mark as visited
		visited[vertex_index] = 1;
		
		//for each element in the adjacency list
		for(i = 0; i < adjacent_size; i++)
		{

			//if this is the terminating node in the cycle
			if(adjacent[i] == initial)
			{
				if(vertex_index < NLOCK)
				{
					kprintf("lockid=%d ",vertex_index);
				}

				else
				{
					kprintf("pid=%d ",vertex_index-NLOCK);
				}

				return 1;
			}
			
			//if not terminating node, recurse
			else if(visited[adjacent[i]] == 0 && printAdjacent(adjacent[i], initial, visited) == 1)
			{
				if(vertex_index < NLOCK)
				{
					kprintf("lockid=%d ",vertex_index);
				}

				else
				{
					kprintf("pid=%d ",vertex_index-NLOCK);
				}
				return 1;
			}
		}
	}
	
	visited[vertex_index] = 0;
	return 0;
}

/**
 * A helper method to recursively check for cycles in a graph
 *
 * @param int	index
 * @param int*	visited
 * @param int*	stack
 */
int isCycle(int vertex_index, int *visited, int *stack)
{
	int i,j,k;
	int n = NPROC + NLOCK;
	int adjacent[n];
	int adjacent_size = 0;

	//fill adjacent with all adjacent nodes for a given vertex 
	for(i = 0; i < n; i++)
	{
		//if there is an edge
		if(rag[vertex_index][i] == 1)
		{
			adjacent[adjacent_size] = i;
			adjacent_size++;
		}
	}

	if(visited[vertex_index] == 0)
	{
		//mark the current vertex as visited and add it to stack
		visited[vertex_index] = 1;
		stack[vertex_index] = 1;

		//recurse over all connected vertices
		for(i = 0; i < adjacent_size; i++)
		{
			//if not yet visited and is part of a cycle
			if(visited[adjacent[i]] == 0 && isCycle(adjacent[i], visited, stack) == 1)
			{				
				return 1;
			}

			//absolute base case --> when we find a backedge
			else if(stack[adjacent[i]] == 1)
			{

				//visited vertices for print 
				int visited_print[n];
				
				//hold the index of the killed process
				lid32 block_lock;
			
				//new visited for printing
				for(j = 0; j < n; j++)
				{
					visited_print[j] = 0;
				}
	
				//call the recursive method to print
				kprintf("\nDEADLOCK: ");
				
				printAdjacent(vertex_index, vertex_index, visited_print);
				

				/* ***DEADLOCK RECOVERY*** */
				
				//a lock
				if(vertex_index < NLOCK)
				{
					deadlock_recover(vertex_index, adjacent[i]);
				}

				//a processs
				else if(vertex_index >= NLOCK)
				{
					
					for(j = 0; j < NLOCK; j++)
					{
						if(rag[j][vertex_index] == 1)
						{
							block_lock = j;	
						}
					}
					
					deadlock_recover(block_lock, vertex_index);
				}				
				
	
				return 1;
			}
		}
	}

	//remove vertex @ index from stack
	stack[vertex_index] = 0;
	return 0;
}

/**
 * Attempts to recover from a deadlock situation
 *
 * @param int 			lockid		
 * @param pid32 		adjacent
 * @return pid32		killed lockid
 */
void deadlock_recover(lid32 lockid, pid32 kid)
{
	struct 	lockentry *lptr;	
	struct 	lockentry *adjacent_entry_lptr;
	pid32	pid_index = kid - NLOCK;
	int 	i;
	
	//grab lock that corresponds to the deadlocked lockid
	lptr = &locktab[lockid];

	//kill the process
	struct	procent *prptr;
	prptr = &proctab[pid_index];

	mutex_unlock(&lptr->lock);

	//zero out request and allocation edges and remove pid from other locks' waiting queues
	for(i = 0; i < NLOCK; i++)
	{		
		//remove from the lock queue
		if(rag[kid][i] == 1)
		{
			adjacent_entry_lptr = &locktab[i];
			remove(kid, adjacent_entry_lptr->wait_queue);
			
			//remove the request edge
			rag[kid][i] = 0;
		}
		
		//get rid of all allocation edges
		if(rag[i][kid] == 1)
		{
			rag_dealloc(kid, i);	
		}
	}
	
	//unlock the lock
	kid = pid_index;

	kprintf("\nDEADLOCK RECOVER: killing pid=%d to release lockid=%d\n\n", kid, lockid); 
	
	//kill the process
	kill(pid_index);

	return;
}
