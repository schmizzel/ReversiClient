#include "utilities.h"
#include <string.h>

void panic (char* message) {
    perror(message);
    exit (EXIT_FAILURE);
}

char* copyStringToNewMemoryAddr(const char* str){
    char* newString = calloc((strlen(str) + 1),sizeof(char));
    if (newString == NULL){
        perror("Failed to allocate memory");
    }
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

char** slice(const char* str, char delimiter[]) {
    char* copy = copyStringToNewMemoryAddr(str);
    char** result = NULL;
    char* token = strtok(copy, delimiter);
    int delimiterC = 0;

    while(token) {
        result = realloc(result, sizeof(char*) * ++delimiterC);

        if(result == NULL) {
            panic("Cannot realloc to split string to char**\n");
            exit(EXIT_FAILURE);
        }

        result[delimiterC - 1] = token;

        token = strtok(NULL, delimiter);
    }

    result = realloc(result, sizeof(char*) * (delimiterC + 1));
    result[delimiterC] = 0;
    free(copy);

    return result;
}

int sliceLength(char** slice) {
    int length = 0;
    while(*slice[length++] != '\0') {}

    return length;
}