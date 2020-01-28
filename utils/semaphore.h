#ifndef READING_ROOM_SEMAPHORE_H
#define READING_ROOM_SEMAPHORE_H

#include "definitions.h"
#include <sys/sem.h>

int semaphore;

struct sembuf
        bookshelfSemaphore = {0, 0, SEM_UNDO},
        readingRoomSemaphore = {CAPACITY, 0, SEM_UNDO};

#define READER 1
#define WRITER PEOPLE

void enterAs(int role) {
    readingRoomSemaphore.sem_op = (short) -role;
    if (semop(semaphore, &readingRoomSemaphore, 1) == -1)
        errorExitWithStatus("Entering Reading Room", role);
}

void leaveAs(int role) {
    readingRoomSemaphore.sem_op = (short) +role;
    if (semop(semaphore, &readingRoomSemaphore, 1) == -1)
        errorExitWithStatus("Leaving Reading Room", role);
}

void requestAccess(int slot) {
    bookshelfSemaphore.sem_num = slot;
    bookshelfSemaphore.sem_op = -1;
    if (semop(semaphore, &bookshelfSemaphore, 1) == -1)
        errorExitWithStatus("Requesting Access To Book Slot", slot + 1);
}

void returnAccess(int slot) {
    bookshelfSemaphore.sem_num = slot;
    bookshelfSemaphore.sem_op = +1;
    if (semop(semaphore, &bookshelfSemaphore, 1) == -1)
        errorExitWithStatus("Returning Access To Book Slot", slot + 1);
}

void obtainSemaphore() {
    semaphore = semget(KEY, CAPACITY + 1, IPC_CREAT | 0600);
    if (semaphore == -1) errorExit("Creating Semaphores");
}

void initializeSemaphore() {
    obtainSemaphore();
    for (int b = 0; b < CAPACITY; ++b)
        if (semctl(semaphore, b, SETVAL, 1) == -1)
            errorExitWithStatus("Initializing Bookshelf Semaphore", b + 1);

    if (semctl(semaphore, CAPACITY, SETVAL, PEOPLE) == -1)
        errorExit("Initializing Reading Room Semaphore");
}

#endif //READING_ROOM_SEMAPHORE_H
