// add an "extern" declaration for the RAG
extern int32 rag[NLOCK + NPROC][NLOCK + NPROC];

// add your deadlock function prototypes
void rag_request(int pid, int lockid);
void rag_alloc(int pid, int lockid);
void rag_dealloc(int pid, int lockid);
void deadlock_detect(void);
int isCycle(int index, int *visited, int *stack);
int printAdjacent(int vertex_index, int initial, int *visited);
void deadlock_recover(lid32 lockid, pid32 kill_proc);
