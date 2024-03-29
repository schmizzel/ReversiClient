#include "misc/utilities.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

void panic (char* message) {
    perror(message);
    exit (EXIT_FAILURE);
}

void* safeMalloc(size_t size){
    void* ptr = malloc(size);
    if (ptr == NULL)
        panic("Failed to malloc\n");
    return ptr;
}

void logMessage(char* message, int level){
    if (level <= LOG_LEVEL)
        printf("%s\n",message);
}

char* copyStringToNewMemoryAddr(const char* str){
    char* newString = safeMalloc((strlen(str) + 1) * sizeof(char));
    strcpy(newString,str);
    return newString;
}

char* concatStringToNewMemoryAddr(const char* str1, const char* str2, const char* divider){
    char* out = calloc((strlen(str1) + strlen(str2) + strlen(divider) + 1), sizeof(char));
    strcat(out, str1);
    strcat(out, divider);
    strcat(out, str2);
    return out;
}

void printStringWithTerminator(const char* str){
    int length = strlen(str);
    for (int i = 0; i <= length; i++){
        if (str[i] == '\0')
            printf("NULLTERM");
        else
            printf("%c",str[i]);
    }
    printf("\n");
}

void freeArrayWithContents(void** arr, size_t length){
    for (size_t i = 0; i < length; i++){
        free(arr[i]);
    }
    free(arr);
}

//TODO: Remove Me?
char** sliceLimit(const char* str, char *delimiter, size_t* lengthOut, int limit) {
    char *copy = copyStringToNewMemoryAddr(str);

    char** result = NULL;
    char* token = strtok(copy, delimiter);

    unsigned long length = strlen(str);
    unsigned long index = 0;
    while(token && limit-- > 0) {
        result = realloc(result, sizeof(char*) * ++(*lengthOut));

        if(result == NULL)
            panic("Cannot realloc to split string to char**\n");

        result[*lengthOut - 1] = token;
        index += strlen(token) + 1;
        token = strtok(NULL, delimiter);
    }

    if(token && index < length) {
        result = realloc(result, sizeof(char*) * ++(*lengthOut));
        result[*lengthOut - 1] = copy + index;
    }
    return result;
}
    
char** slice(const char* str, char *delimiter, size_t* lengthOut) {

    char* copy = copyStringToNewMemoryAddr(str);

    char** result = NULL;
    char* token = strtok(copy, delimiter);

    *lengthOut = 0;

    while(token) {
        result = realloc(result, sizeof(char*) * ++(*lengthOut));

        if(result == NULL)
            panic("Cannot realloc to split string to char**\n");


        result[*lengthOut - 1] = token;

        token = strtok(NULL, delimiter);
    }
    return result;
}


char* newStringWithoutDelimiter(const char* str, char delimiter){
    char* out = NULL;
    size_t length = 0;
    for (size_t i = 0; i < strlen(str); i++){
        if (str[i] != delimiter){
            out = realloc(out, sizeof(char) * ++length);
            out[length - 1] = str[i];
        }
    }

    out = realloc(out, sizeof(char) * ++length);
    out[length - 1] = '\0';
    return out;
}

void freeTokens(char** tokens){
    free(tokens[0]);
    free(tokens);
}

char* joinTokens(char** tokens, size_t length, char* delimiter){
    size_t newLength = 1;
    for (size_t i = 0; i < length; i++){
        newLength += strlen(tokens[i]) + strlen(delimiter);
    }
    newLength -= strlen(delimiter);
    char* out = calloc(newLength, sizeof(char));
    for (size_t i = 0; i < length; i++){
        strcat(out, tokens[i]);
        if (i  != length - 1)
            strcat(out, delimiter);
    }
    return out;
}

double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int getRandomInt(int min, int max){
    return min + rand() % max;
}

void exitWithExitCode(int err){
    if (err == -1)
        exit(EXIT_FAILURE);
    else 
        exit(EXIT_SUCCESS);
}

int fileDescriptorReadIsReady(int fd, int timeoutInSeconds){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);
    struct timeval timeout;
    timeout.tv_sec = timeoutInSeconds;
    timeout.tv_usec = 0;
    int ret = select(fd+1,&rfds,NULL,NULL,&timeout);
    if (ret == -1)
        panic("Select Failed!");
    
    return ret;
}

long long currentTimestampInMs(){
    struct timeval te; 
    gettimeofday(&te, NULL); 
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; 
    return milliseconds;
}