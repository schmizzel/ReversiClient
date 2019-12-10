#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

typedef struct _ProcessInfo {
    pid_t child, parent;
    int *fd;
} ProcessInfo;

ProcessInfo* createProcessInfo();

void setChild(ProcessInfo* info, pid_t pid);

void setParent(ProcessInfo* info, pid_t pid);

pid_t getChild(ProcessInfo* info) {
    return info->child;
}

pid_t getParent(ProcessInfo* info) {
    return info->parent;
}

int readFileDescriptor(ProcessInfo* info);

int writeFileDescriptor(ProcessInfo* info);

#endif
