#ifndef GAMESEQUENCE_H
#define GAMESEQUENCE_H

    #include "shareddataaccess/boarddataaccess.h"
    #include "communicator/connection.h"

    int startGameLoop(Connection* connection, BoardSHM* boardSHM, int moveTime);

#endif