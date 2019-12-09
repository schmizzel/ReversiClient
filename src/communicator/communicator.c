#include "communicator_priv.h"

//TODO: Parse Server Messages and wrap responses in ServerMessageType
//TODO: Cache if wrong response type
//TODO: use readLine instead of readMessage
char* getServerGreeting(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    
    char* out = copyStringToNewMemoryAddr(message->messageReference + 2); 
    freeServerMessage(message);
    return out;
}

int hasAcceptedVersion(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    int result = 1;
    if (message->type == Error)
        result = 0;
    freeServerMessage(message);
    return 1;
}

GameKind getGameKind(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    GameKind out = parseGameKind(message->messageReference);
    freeServerMessage(message);
    return out;
}

GameKind parseGameKind(char* message){
    if (strcmp(message,REVERSI_GAMEKIND) == 0){
        return gamekind_Reversi;
    } else {
        return gamekind_Unkown;
    }
}

char* getGameName(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    char* out = copyStringToNewMemoryAddr(message->messageReference + 2);
    freeServerMessage(message);
    return out;
}

PlayerMeta* getPlayerMeta(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    PlayerMeta* out = parsePlayerMeta(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* parsePlayerMeta(char* message){
    size_t length = 0;
    char** tokens = slice(message," ",&length);
    
    //TODO: find better way to do this!
    if (length < 4)
        panic("unexpected playermeta string, expected 4 tokens");

    int number = atoi(tokens[2]);
    char* name = getNameFromPlayerMetaTokens(tokens,length);
    PlayerMeta* out = newPlayerMeta(number,name,1);
    freeTokens(tokens);
    return out;
}

char* getNameFromPlayerMetaTokens(char** tokens, size_t tokenCount){
    return joinTokens(&tokens[3],tokenCount - 3, " ");
}

PlayerMeta* getOtherPlayer(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    PlayerMeta* out = parseOtherPlayerMeta(message->messageReference);
    freeServerMessage(message);
    return out;
}

PlayerMeta* parseOtherPlayerMeta(char* message){
    size_t length = 0;
    char** tokens = slice(message," ",&length);
    
    //TODO: find better way to do this!
    if (length < 4)
        panic("unexpected playermeta string, expected 4 tokens");

    int number = atoi(tokens[1]);
    char* name = getNameForOtherPlayersTokens(tokens,length);
    int isReady = atoi(tokens[length - 1]);
    PlayerMeta* out = newPlayerMeta(number,name, isReady);
    freeTokens(tokens);
    return out;
}

char* getNameForOtherPlayersTokens(char** tokens, size_t tokenCount){
    size_t nameTokenCount = tokenCount - 3;
    char* nameTokens[nameTokenCount]; 
    size_t index = 0;
    for (size_t i = 2; i < tokenCount - 1; i++){
        nameTokens[index] = tokens[i];
        index++;
    }
    return joinTokens(nameTokens,nameTokenCount," ");
}

int getTotalPlayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        return -1;

    char ASCInum = message->messageReference[8];
    return ASCInum - '0';
}

int nextMessageIsEndplayers(Connection* connection){
    ServerMessage* message = receiveServerMessage(connection);
    if (message->type == Error)
        panic(message->messageReference);
    int result = 0;
    if (message->type == Error)
        panic(message->messageReference);
    else if (strcmp(message->messageReference, ENDPLAYERS_COMMAND) == 0)
        result = 1;

    freeServerMessage(message);
    return result;
}

void sendClientVersion(Connection* connection, const char* version){
    char* message = concatStringToNewMemoryAddr(VERSION,version," ");
    printf("Sending: '%s'\n",message);
    writeLineToServer(connection, message);
    free(message);
}

void sendGameId(Connection* connection, const char* gameID){
    char* message = concatStringToNewMemoryAddr(GAMEID,gameID," ");
    printf("Sending: '%s'\n",message);
    writeLineToServer(connection, message);
    free(message);
}

void sendPlayerPreference(Connection* connection, const char* preference){
    if (preference == NULL){
        printf("Sending: '%s'\n",PLAYERPREFERENCE);
        writeLineToServer(connection, PLAYERPREFERENCE);
    } else {
        char* message = concatStringToNewMemoryAddr(PLAYERPREFERENCE,preference," ");
        printf("Sending: '%s'\n",message);
        writeLineToServer(connection, message);
        free(message);
    }
}

PlayerMeta* newPlayerMeta(int number, char* name, int isReady){
    PlayerMeta* meta = malloc(sizeof(PlayerMeta));
    meta->name = copyStringToNewMemoryAddr(name);
    meta->number = number;
    meta->isReady = isReady;
    return meta;
}

void freePlayerMeta(PlayerMeta* meta){
    free(meta->name);
    free(meta);
}