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
#include "../echange.c"





struct client_init_infos init_struct;

int client_setup(char *ip, int port, int nb_joueurs, int socket_client);

void init(int server_socket, clients *list);

int Server_setup(int port);



