#ifndef READING_ROOM_MESSAGE_QUEUE_H
#define READING_ROOM_MESSAGE_QUEUE_H

#include "definitions.h"
#include <sys/msg.h>

int messageQueue;

void obtainMessageQueue() {
    messageQueue = msgget(KEY, IPC_CREAT | 0600);
    if (messageQueue == -1) errorExit("Creating Message Queue");
}

struct {
    long type;
    int slot, title;
} book;

#define EMPTY_BOOK_SLOT PEOPLE

const int messageSize = sizeof(book) - sizeof(long);

void send(int ID, int slot) {
    book.type = ID + 1;
    book.slot = slot;
    if (msgsnd(messageQueue, &book, messageSize, 0) == -1)
        errorExitWithStatus("Sending Message", slot + 1);
}

int receive(int ID) {
    return msgrcv(messageQueue, &book, messageSize, ID + 1, IPC_NOWAIT) == messageSize;
}

void initializeMessageQueue() {
    obtainMessageQueue();
    int bytes;
    while ((bytes = msgrcv(messageQueue, &book, INT32_MAX, 0, IPC_NOWAIT)) != -1)
        fprintf(stderr, "Read %d bytes from book queue\n", bytes);
    fprintf(stderr, "Message Queue Has Been Cleared\n");
    for (int b = 0; b < CAPACITY; ++b) send(EMPTY_BOOK_SLOT, b);
}

#endif //READING_ROOM_MESSAGEQUEUE_H
