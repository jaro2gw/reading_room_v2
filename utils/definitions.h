#ifndef READING_ROOM_DEFINITIONS_H
#define READING_ROOM_DEFINITIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void errorExitWithStatus(const char *message, int status) {
    perror(message);
    exit(status);
}

void errorExit(const char *message) {
    errorExitWithStatus(message, 1);
}

#define KEY 765432
#define PEOPLE 25
#define CAPACITY 5
#define PERSON_TAG "[%02d]"
#define BOOK_TAG "<%02d> \"%s\""

#endif //READING_ROOM_DEFINITIONS_H
