/**
 * File-Transfer-System
 * transfer file from client-side to server-side using OIOSocket.
 * This is the serve-side code.
 * Copyright Zephyro 2012
 */
#include "network.h"

using namespace std;

/**
 * output buf to file.
 * params: 
 * 	buf 		content
 *	len 		length of content
 * 	fileName 	file that need to output
 * 	newfile		append or create a new file
 * return:
 *	positive 	success
 * 	negtive		fail
 */
int filewrite(const char buf[], const int len, const char* fileName,
	      bool newfile) {
  FILE *pFile;
  pFile = fopen(fileName, newfile ? "wb" : "ab");	
  if (fwrite(buf, 1, len, pFile) < 0) {
    cerr << "file write error " << fileName << endl;
    fclose(pFile);
    return -1;
  }
  fflush(pFile);
  fclose(pFile);
  return 1;
}

/**
 * get the len of input-line buf[], seperate by \0.
 */
int getlen(const char buf[], const int max) {
  int i = 0;
  for (; i < max; ++i) {
    if (buf[i] == '\0') return i - 1;
  }
  return i;
}

char* genFileName(const char* buf, const int len) {
  int i;
  for (i = 0; i < len; ++i) {
    if (buf[i] == '\0' || buf[i] == '\n' || buf[i] == '\r') break;
  }
  char* fileName = (char*) malloc(i);
  memcpy(fileName, buf, i);
  return fileName;
}

/**
 * read from a client-socket, echo back.
 */
void do_read(int cltfd) {
  cout << "start to read" << endl;
  bool isCommand = false;
  char buf[MAXLINE];
  char* fileName;
  ssize_t n;
  while (n = recv(cltfd, buf, MAXLINE, 0) > 0) {
    cout << "read some buffers" << endl;
    int len = getlen(buf, MAXLINE);
    cout << buf << endl;
    if (!isCommand) {
      fileName = genFileName(buf, len);
      isCommand = true;
      filewrite("", 0, fileName, true);
      cout << "create a new file " << fileName << "." << endl;
    } else {
      filewrite(buf, len, fileName, false);
    }
    memset(buf, '\0', MAXLINE);
  }
}

struct sockaddr_in makeSockAddr(const int port) {
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = 0;
  return sin;
}

void doAccept(int fd) {
  cout << "accept a new connection." << endl;
  do_read(fd);
  close(fd);
  cout << "connection lost with " << fd << endl;
  exit(0);
}

int runServer() {
  struct sockaddr_in sin = makeSockAddr(9901);
  
  int listener = socket(AF_INET, SOCK_STREAM, 0);
  
  if (bind(listener, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
    cerr << "error in bind" << endl;
    return -1;
  }
  
  if (listen(listener, 16) < 0) {
    cerr << "error in listener" << endl;
    return -1;
  }
  
  struct sockaddr_in clt;
  int cltfd;
  socklen_t cltlen = sizeof(clt);
  while ((cltfd = 
	  accept(listener, 
		 (struct sockaddr*) &clt, &cltlen)) > 0) {
    int pid = fork();
    switch(pid) {
    case -1: 
      cerr << "fork error" << endl;
      exit(-1);
    case 0:
      doAccept(cltfd);
    default:
      break;
    }
  }

  close(listener);
  cout << "finish" << endl;
  return 1;
}

int main() {
  cout << "***************************************" << endl;
  cout << "File Transfer System by Francois" << endl;
  cout << "***************************************" << endl;
  return runServer();
}
