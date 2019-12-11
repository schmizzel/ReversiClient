#include "info/process.h"
#include "utilities.h"
#include <stdlib.h>
#include <unistd.h>

ProcessInfo* createProcessInfo() {
    int fd[2];
    if(pipe(fd) < 0)
        panic("Could not create a pipe");

    ProcessInfo* info = malloc(sizeof(ProcessInfo));
    info->fd[0] = fd[0];
    info->fd[1] = fd[1];
    info->parent = malloc(sizeof(pid_t*));
    info->child = malloc(sizeof(pid_t*));
    *info->child = -1;
    *info->parent = -1;

    return info;
}

void freeProcessInfo(ProcessInfo* procInfo) {
    free(procInfo->parent);
    free(procInfo->child);
    free(procInfo);
}

void setProcChild(ProcessInfo* info, const pid_t* pid) {
    *info->child = *pid;
}

void setProcParent(ProcessInfo* info, const pid_t* pid) {
    *info->parent = *pid;
}

pid_t getProcChild(ProcessInfo* info) {
    return *info->child;
}

pid_t getProcParent(ProcessInfo* info) {
    return *info->parent;
}

int readFileDescriptor(ProcessInfo* info) {
    return info->fd[0];
}

int writeFileDescriptor(ProcessInfo* info) {
    return info->fd[1];
}