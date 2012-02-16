#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

// for fork()
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1400

int filewrite(const char buf[], const int len, const char* fileName, bool isNewFile);
