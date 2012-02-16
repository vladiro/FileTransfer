/**
 * File-Transfer-System
 * transfer file from client-side to server-side using OIOSocket.
 * This is the client-side code.
 * Copyright Zephyro 2012
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

struct buffer {
  const char* content;
  int len;
};

struct buffer getContent(const char* fileName) {
  struct buffer b;
  FILE *pFile = fopen(fileName, "rb");
  char *buf = (char *) malloc(sizeof(char) * 1400);
  if (fread(buf, 1, 1400, pFile) < 0) {
    cerr << "read file " << fileName << " error." << endl;
    exit(-1);
  }
  b.content = buf;
  b.len = strlen(b.content);
  return b;
}

void run(const char* fileName, const char* destFileName) {
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(9901);
  sin.sin_addr.s_addr = 0;

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  int con;
  if(con = connect(fd, (struct sockaddr *) &sin,sizeof(sin)) == -1) {
    cerr << "error in connect." << endl;
    exit(-1);
  }
  
  cout << "connect builded." << endl;
  if (con >= 0) {
    buffer b = getContent(fileName);
    char* destFileName0 = new char[strlen(destFileName) + 2];
    sprintf(destFileName0, "%s\r\n", destFileName);
    if (write(fd, destFileName0, strlen(destFileName0)) < 0) {
      cerr << "error in write" << endl;
      exit(1);
    }
    if (write(fd, b.content, b.len) < 0) {
      cerr << "error in write 2" << endl;
      exit(1);
    }
    cout << "both write finished." << endl;
  
    close(fd);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cerr << "check the args please." << endl;
    exit(-1);
  }
  char* fileName = (char *) argv[1];
  char* destFileName = (char *) argv[2];
  run(fileName, destFileName);
  return 0;
}
 
