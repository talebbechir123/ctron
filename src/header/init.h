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






int disc_set(int socket_serv, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, clients *list);


void set(display_info *board);


void set_pos(clients *clients_list);
