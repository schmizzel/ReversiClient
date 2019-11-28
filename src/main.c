#include "optionreader.h"
#include "communicator/connection.h"
#include "communicator/prolog.h"
#include "thinker/thinker.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define PORTNUMBER "1357"
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define VERSION_NUMBER "2.3"

int main(int argc, char *argv[]) {
    char* gameId = readGameID(argc,argv);
    if (gameId == NULL) {
        printf("GameId must be set!\n");
        return EXIT_FAILURE;
    }
    char* playerPreference = readPreferencedPlayerNumber(argc,argv);

    pid_t thinker, connector;
    int fd[2];

    if(pipe(fd) < 0) {
        perror("Could not create pipe");
        exit (EXIT_FAILURE);
    }
    if((thinker = fork()) < 0) {
        perror("Could not fork for thinker process");
        exit(EXIT_FAILURE);
    }

    if(thinker > 0) {
        printf("[PARENT/%i] Started thinker child\n", thinker);
        if((connector = fork()) < 0) {
            perror("Could not fork for connector process");
            exit(EXIT_FAILURE);
        }

        // still in the parent process
        if(connector > 0) {
            printf("[PARENT/%i] Started connector child\n", connector);
            // still in the parent process

            if ((waitpid (thinker, NULL, 0)) < 0 || waitpid(connector, NULL, 0) < 0) {
                perror ("Error waiting for child processes to die");
                exit (EXIT_FAILURE);
            }
            printf("Thinker and connector died, exiting..\n");
        } else {
            printf("[CHILD/CONNECTOR] Starting connector..\n");
            // start the connector process
            Connection* connection = newConnection(HOSTNAME,PORTNUMBER);
            connectToServer(connection);
            initiateProlog(connection,VERSION_NUMBER,gameId, playerPreference);
            disconnectFromServer(connection);
            freeConnection(connection);
            free(gameId);
            free(playerPreference);

            kill(thinker, SIGUSR1);
            return EXIT_SUCCESS;
        }
    } else {
        // start the thinker process
        printf("[CHILD/THINKER] Starting thinker..\n");
        tick(NULL, fd);
    }
}