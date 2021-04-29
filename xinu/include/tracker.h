#define NUM_BUFS 4

struct bpentry *speedbuffer;
bpid32 speedbufpid;
struct bpentry *speedbufpool[NUM_BUFS];
int currbuf;