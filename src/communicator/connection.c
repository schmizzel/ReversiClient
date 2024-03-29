#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include "misc/utilities.h"
#include "communicator/connection.h"

struct addrinfo* getSocketAddr(const char* hostname, const char* port);

struct _Connection {
    char* hostname;
    char* port;
    int socket;
    int timeoutInSec;
};

Connection* newConnection(const char* hostname, const char* port,int timeoutInSec){
    Connection* connection = safeMalloc(sizeof(Connection));
    connection->hostname = copyStringToNewMemoryAddr(hostname);
    connection->port = copyStringToNewMemoryAddr(port);
    connection->socket = -1;
    connection->timeoutInSec = timeoutInSec;
    return connection;
}

void freeConnection(Connection* connection) {
    free(connection->hostname);
    free(connection->port);
    free(connection);
}

int connectToServer(Connection* connection){    
    struct addrinfo* socketAddr = getSocketAddr(connection->hostname, connection->port);
    if (socketAddr == NULL){
        printf("failed to get address information");
        return -1;
    }
        

    int sock = socket(socketAddr->ai_family,socketAddr->ai_socktype,socketAddr->ai_protocol);
    if (sock == -1){
        printf("failed to create socket");
        return -1;
    }
        

    if (connect(sock,socketAddr->ai_addr,socketAddr->ai_addrlen) == -1){
        printf("failed to connect to socket");
        return -1;
    }
        
    freeaddrinfo(socketAddr);
    connection->socket = sock;
    printf("Successfully Connected to host '%s:%s'\n", connection->hostname, connection->port);
    return 0;
}

int disconnectFromServer(Connection* connection){
    if (connection->socket != -1){
        if (close(connection->socket) == -1){
            printf("Failed To close Socket");
            return -1;
        }
    }
    connection->socket = -1;
    return 0;
}

ServerMessage* receiveServerMessage(Connection* connection) {
    return parseServerMessage(readLineFromServer(connection));
}

char* readLineFromServer(Connection* connection){
    if (!fileDescriptorReadIsReady(connection->socket,connection->timeoutInSec))
        panic("Server Timeout!");

    ssize_t length = 0;
    char currentChar = 'a';
    char* result = NULL;
    while(currentChar != '\0'){
        ssize_t readLength = read(connection->socket,&currentChar,1);
        
        if (readLength == -1){
            panic("Failed to read From Server");
        } else if (readLength == 0 || currentChar == '\n'){
            currentChar = '\0';
        }
        
        result = realloc(result, sizeof(char) * ++length);
        if(result == NULL) 
            panic("Failed to realloc");

        result[length - 1] = currentChar;
    } 
    return result;
}

void writeLineToServer(Connection* connection, char* message){
    if (connection->socket == -1)
        panic("Not connectet to server");
    
    char* in = concatStringToNewMemoryAddr(message,"\n","");
    ssize_t len;
    if ((len = write(connection->socket, in, strlen(in))) == -1)
        panic("Failed to write message to server");
    free(in);
}

struct addrinfo* getSocketAddr(const char* hostname, const char* port){
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    struct addrinfo* res = NULL;
    int err = getaddrinfo(hostname,port,&hints,&res);
    if (err != 0) {
        freeaddrinfo(res);
        return NULL;
    }
    return res;
}