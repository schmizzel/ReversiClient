#ifndef GAMEDATAACCESS_H
#define GAMEDATAACCESS_H
    #include "core.h"
    #include <stdlib.h>

    typedef struct _GameDataSHM GameDataSHM;

    GameDataSHM* createGameDataSHM();
    int detachGameDataSHM(GameDataSHM* sharedMemory);
    void freeGameDataSHM(GameDataSHM* shm);
    int clearGameDataSHM(GameDataSHM* sharedMemory);
    void setGameName(GameDataSHM* shm, char* gameName);
    char* getGameName(GameDataSHM* shm);
    void setProcessInfo(GameDataSHM* shm, ProcessInfo* processInfo);
    ProcessInfo* getProcessInfo(GameDataSHM* shm);
    int getIsThinking(GameDataSHM* shm);
    void setIsThinking(GameDataSHM* shm, int val);
    int getMoveTime(GameDataSHM* shm);
    void setMoveTime(GameDataSHM* shm, int val);
    void setOwnPlayerMeta(GameDataSHM* shm, PlayerMeta* meta);
    PlayerMeta* getOwnPlayerMeta(GameDataSHM* shm);
    void setOpponenPlayerMeta(GameDataSHM* shm, PlayerMeta* meta, size_t index);
    PlayerMeta* getOpponentPlayerMeta(GameDataSHM* shm, size_t index);
    void setOpponenCount(GameDataSHM* shm, size_t count);
    size_t getOpponenCount(GameDataSHM* shm);


    
#endif