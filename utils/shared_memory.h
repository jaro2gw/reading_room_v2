#ifndef READING_ROOM_SHARED_MEMORY_H
#define READING_ROOM_SHARED_MEMORY_H

#include "definitions.h"
#include <sys/shm.h>

bool *currentlyReader;
short *readersCount;

void attachReaders() {
    int sharedMemory = shmget(KEY, PEOPLE * sizeof(bool), IPC_CREAT | 0600);
    if (sharedMemory == -1) errorExitWithStatus("Getting Current Readers Memory Segment", 1);
    currentlyReader = shmat(sharedMemory, NULL, 0);
    if (currentlyReader == (void *) -1) errorExitWithStatus("Attaching Current Readers Array", 1);
}

void attachBookshelf() {
    int sharedMemory = shmget(KEY + 1, CAPACITY * sizeof(short), IPC_CREAT | 0600);
    if (sharedMemory == -1) errorExitWithStatus("Getting Bookshelf Memory Segment", 1);
    readersCount = shmat(sharedMemory, NULL, 0);
    if (readersCount == (void *) -1) errorExitWithStatus("Attaching Bookshelf Array", 1);
}

#endif //READING_ROOM_SHAREDMEMORY_H
