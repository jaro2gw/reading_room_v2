#include <wait.h>
#include "utils.h"

int main() {
    initializeSemaphore();
    initializeMessageQueue();

    int pid[PEOPLE];
    for (int person = 0; person < PEOPLE; ++person) {
        switch (pid[person] = fork()) {
            case -1:
                errorExit("Forking Process");
            case 0: {
                char ID[11];
                sprintf(ID, "%d", person);
                execl("person_v2.exe", "person_v2", ID, NULL);
                errorExit("Running person.exe");
            }
            default:
                fprintf(stderr, PERSON_TAG" is associated with (process %d)\n", person, pid[person]);
        }
    }

    for (int i = 0, status, process, person; i < PEOPLE; ++i) {
        process = wait(&status);
        person = -1;
        for (int p = 0; p < PEOPLE; ++p)
            if (pid[p] == process)
                person = p;
        fprintf(stderr, PERSON_TAG" (process %d) finished with status %d\n", person, process, status);
    }

    return 1;
}