#ifndef __COMMON_H__
#define __COMMON_H__


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

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define TRAIL_UP 4

#define SERV_PORT 5555
#define XMAX 100
#define YMAX 50
#define TIE -2
#define TRAIL_INDEX_SHIFT 50

#define SA struct sockaddr
#define SAI struct sockaddr_in
#define TV struct timeval

#define WALL 111

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })


#define CHECK(x)                                            \
    do                                                      \
    {                                                       \
        if (!(x))                                           \
        {                                                   \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x);                                     \
            exit(EXIT_FAILURE);                             \
        }                                                   \
    } while (0)


typedef struct display_info
{
    char board[XMAX][YMAX];
    int winner;
} display_info;


struct client_input {
    int id;
    char input;
};

struct client_init_infos {
    int nb_players;
};




/*
*  strutcture  position d'un joueur
* */
typedef struct position
{
    int x; // position x du joueur
    int y; // position y du joueur
    int trail; // 1 si le joueur est en train de tracer un trait, 0 sinon
} position;

/*
*  structure joueur
* */
typedef struct player
{
    int id; // id du joueur
    int fd; // fd du joueur
    position pos; // position du joueur
} player;
#define MAXCLIENTS 2

/*
*  structure liste des joueurs
* */
typedef struct clients
{
    player players[MAXCLIENTS]; // liste des joueurs
    int nb_clients;            // nombre de joueurs connectés
} clients;

// nombre de joueurs connectés
int nb_client = 0;
// nombre de terminaux connectés
int nb_terminal = 0;


#endif