#ifndef PROLOG_H
#define PROLOG_H

#include "communicator/communicator.h"

void initiateProlog(Connection* connection, const char* version, const char* gameId, const char* playerPreference);

#endif