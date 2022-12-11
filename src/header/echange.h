#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/time.h>
#include <ncurses.h>
#include <termios.h>
#include <time.h>
#include "common.h"


int server_send(int client_socket, void *data);

int server_recv(int client_socket, void *data);