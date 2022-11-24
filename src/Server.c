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
#include <sys/select.h>
#include <fcntl.h>

#define MAXCLIENTS 2

typedef struct position
{
    int x;
    int y;
    int trail;
} position;

typedef struct player
{
    int id;
    char Key;
    position pos;
} player;
typedef struct clients
{
    player players[MAXCLIENTS];
    int nb_clients;
} clients;

// nombre de joueurs
int nb_client = 0;
// nombre de terminaux connectés
int nb_terminal = 0;

int server_send(int client_socket, void *data)
{
    int n = 0;
    int len = strlen(data);
    if ((n = send(client_socket, data, len, 0)) > 0)
    {
    }
    else
    {
        perror("Error : send failed");
        return -1;
    }

    return n;
}
int server_recv(int client_socket, void *data)
{
    int n = 0;
    memset(data, 0, strlen(data));

    if ((n = recv(client_socket, data, sizeof(data), 0)) > 0)
    {
        // process_recv_data(client_socket, recv_msg);
    }
    else
    {
        printf("ERROR: recv failed\n");
    }
    return 0;
}

void init(int server_socket, clients *list)
{

    // list->players[nb_client].id
    struct sockaddr_in client_addr;
    struct client_init_infos init;
    socklen_t client_addr_len = sizeof(client_addr);
    if ((list->players[nb_client].id = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
    {
        perror("ERROR : accept failed");
    }
    int n = server_recv(list->players[nb_client].id, &init);
    if (n == -1)
    {
        perror("ERROR : recv failed");
    }
    // int n;if()
    //  if((n = recv(socket_client, &init, sizeof(struct client_init_infos), 0)) == -1){
    //   perror("ERROR : recv failed");
    //   return -1;
    //  }

    // nb_client=nb_client + init.nb_players;
    list->nb_clients = list->nb_clients + init.nb_players;
    nb_client = nb_client + 1;

    printf("nb_client : %d\n", list->nb_clients);
}

int Server_setup(int port)
{

    int server_socket;
    struct sockaddr_in server_addr;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("ERROR : socket creation failed");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("ERROR : socket binding failed");
        return -1;
    }
    if (listen(server_socket, 5) == -1)
    {
        perror("ERROR : listen failed");
        return -1;
    }
    printf("Server listening on port %d \n", port);
    return server_socket;
}

int disc_set(int socket_serv, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, clients *list)
{
    int maxfd = socket_serv;
    FD_ZERO(readfds);
    FD_ZERO(writefds);
    FD_ZERO(exceptfds);
    FD_SET(socket_serv, readfds);
    for (int i = 0; i < list->nb_clients; i++)
    {
        FD_SET(list->players[i].id, readfds);
        if (list->players[i].id > maxfd)
        {
            maxfd = list->players[i].id;
        }
    }
    return maxfd;
}

void upadte_board(display_info *board, struct client_input input, position *player)
{
    board->board[player->x][player->y] = TRAIL_INDEX_SHIFT + input.id;
    switch (input.input)
    {
    case 'i':
        board->board[player->x][player->y - 1] = input.id;
        player->y = player->y - 1;
        break;
    case 'j':
        board->board[player->x - 1][player->y] = input.id;
        player->x = player->x - 1;

        break;
    case 'k':

        board->board[player->x][player->y + 1] = input.id;
        player->y = player->y + 1;
        // data->board[posx][posy+1]=' ' +  player.id;

        break;
    case 'l':
        board->board[player->x + 1][player->y] = input.id;
        player->x = player->x + 1;
        // data->board[posx+1][posy]= ' ' + player.id;

        break;
    }
}

void set_board(display_info *board)
{
    int i, j;

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            if (i == 0 || i == 15 || j == 0 || j == 15)
            {
                board->board[i][j] = 1;
            }
            else
            {
                board->board[i][j] = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Usage: %s <port> <refresh rate>\n", argv[0]);
        exit(1);
    }

    clients *clients_list = malloc(sizeof(clients));

    int port = atoi(argv[1]);

   // int refresh_rate = atoi(argv[2]);

    int socket_serv = Server_setup(port);

    while (clients_list->nb_clients < 2)
    {

        init(socket_serv, clients_list);
    }

    printf("lets goooo : \n");
    printf("client %d\n", clients_list->players[0].id);
    printf("client %d\n", clients_list->players[1].id);
    printf("server %d\n", socket_serv);
    display_info *data = malloc(sizeof(display_info));
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    //int maxfd = disc_set(socket_serv, &readfds, &writefds, &exceptfds, clients_list);
   
    position *player2 = malloc(sizeof(position));

    data->winner = -1;
    set_board(data);
    player2->x = 7;
    player2->y = 8;
   
        close(socket_serv);
        return 0;
    
}
