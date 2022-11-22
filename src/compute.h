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
#include "common.h"

//function that deterimens the winner of the game
int winner(display_info* data);
//functions that calculate new board after each move
int positon_calcul(display_info* data ,struct client_input player);
