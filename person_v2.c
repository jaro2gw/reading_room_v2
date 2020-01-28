#include <sys/time.h>
#include "utils.h"

int ID;

bool personIsWriter() {
    return !currentlyReader[ID];
}

void roleFeedback() {
    printf(PERSON_TAG" is now a %s\n", ID, personIsWriter() ? "WRITER" : "READER");
}

void changeRole() {
    currentlyReader[ID] = !currentlyReader[ID];
    roleFeedback();
}

void restForABit() {
    sleep(random() % 5 + 1);
    if (personIsWriter()) {
        /* 75% chance for writer to change role to reader */
        if (random() % 4 < 3) changeRole();
    } else {
        /* 25% chance for reader to change role to writer */
        if (random() % 4 < 1) changeRole();
    }
    sleep(random() % 5 + 1);
}

#define BOOK_ARGS book.slot, titles[book.title]

void maybeRead() {
    if (receive(ID)) {
        enterAs(READER);
        printf(PERSON_TAG" is reading "BOOK_TAG"...\n", ID, BOOK_ARGS);
        sleep(random() % 5 + 5); /* Reading a book takes from 5 to 9 seconds. */
        printf(PERSON_TAG" finished reading "BOOK_TAG"\n", ID, BOOK_ARGS);
        requestAccess(book.slot);
        if (--readersCount[book.slot] == 0) {
            printf(BOOK_TAG" has been read by all its readers!\n", BOOK_ARGS);
            send(EMPTY_BOOK_SLOT, book.slot);
        }
        returnAccess(book.slot);
        leaveAs(READER);
    } else printf(PERSON_TAG" did not read any book\n", ID);
}

void maybeWrite() {
    printf(PERSON_TAG" wants to write a new book\n", ID);
    if (receive(EMPTY_BOOK_SLOT)) {
        enterAs(WRITER);
        book.title = (int) random() % TITLES;
        printf(PERSON_TAG" is writing "BOOK_TAG"...\n", ID, BOOK_ARGS);
        sleep(random() % 5 + 5); /* Writing a book takes from 5 to 9 seconds. */
        printf(PERSON_TAG" finished writing "BOOK_TAG"\n", ID, BOOK_ARGS);

        /* Locking on `book.slot` prevents readers from changing `readersCount[book.slot]` too early. */
        requestAccess(book.slot);
        for (int p = 0; p < PEOPLE; ++p) {
            if (currentlyReader[p]) {
                ++readersCount[book.slot];
                send(p, book.slot);
            }
        }
        returnAccess(book.slot);
        leaveAs(WRITER);
    } else printf(PERSON_TAG" will not write a book now\n", ID);
}

int main(int argc, char *argv[]) {
    if (argc < 2) fprintf(stderr, "Please provide a person with ID\n"), exit(1);

    ID = (int) strtol(argv[1], NULL, 10);

    srandom(time(NULL) + ID);

    obtainSemaphore();
    obtainMessageQueue();
    attachReaders();
    attachBookshelf();

    roleFeedback();
    while (1) {
        restForABit();
        maybeRead();
        if (personIsWriter()) maybeWrite();
    }
}