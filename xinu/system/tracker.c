#include <xinu.h>
#include <stdio.h>

float average()
{
    int sum = 0;
    int i;
    for (i = 0; i < NUM_BUFS; i++) {
        sum += speedbufpool[i]->savedspeed;
    }
    float avg = (float)sum / NUM_BUFS;
    return avg;
}

void bufferinit()
{
    speedbufpid = mkbufpool(sizeof(struct bpentry), NUM_BUFS);

    // To get buffer entry of all the buffers of the pool
    int i;
    for (i = 0; i < NUM_BUFS; i++)
    {
        speedbuffer = (struct bpentry *) getbuf(speedbufpid);
        speedbufpool[i] = speedbuffer;
    }

    // Pointer to the current buffer entry
    currbuf = 0;
}

float updatespeed(int speed)
{
    if (currbuf >= NUM_BUFS)
    {
        currbuf = 0;
    }

    speedbufpool[currbuf]->savedspeed = speed;
    currbuf++;

    return average();
}