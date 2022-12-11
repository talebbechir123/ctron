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
    int fd;
    enum Key
    {
        i,
        z
    } key;
    position pos;
} player;
typedef struct clients
{
    player players[MAXCLIENTS];
    int nb_clients;
} clients;

// nombre de joueurs
int nb_client = 0;
int nb_terminal = 0;

int server_send(int client_socket, void *data)
{
    int n = 0;
    int len = strlen(data);
    if ((n = send(client_socket, data, len, 0)) > 0)
    {
    } //
    else
    {
        perror("Error : send failed");
        return -1;
    }
    //
    return n;
}
int server_recv(int client_socket, void *data)
{
    int n = 0;
    memset(data, 0, strlen(data));

    if ((n = recv(client_socket, data, sizeof(data), 0)) > 0)
    {
       
    }
    else
    {
        printf("ERROR: recv failed\n");
    }
    return 0;
}


void init(int server_socket, clients *list)
{

    struct sockaddr_in client_addr;
    struct client_init_infos init;
    socklen_t client_addr_len = sizeof(client_addr);
    list->players[nb_client].id = nb_client;
    if ((list->players[nb_client].fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
    {
        perror("ERROR : accept failed");
    }
    int n = server_recv(list->players[nb_client].fd, &init);
    if (n == -1)
    {
        perror("ERROR : recv failed");
    }

    list->nb_clients = list->nb_clients + init.nb_players;
    nb_client = nb_client + init.nb_players;
    nb_terminal = nb_terminal + 1;

    printf("nb_client : %d\n", list->nb_clients);
    printf("nb_client extern : %d\n", nb_client);
    printf("nb_terminal : %d\n", nb_terminal);
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
        FD_SET(list->players[i].fd, readfds);
        if (list->players[i].fd > maxfd)
        {
            maxfd = list->players[i].fd;
        }
    }
    return maxfd;
}
void check_winner(display_info *board, struct client_input *input, position *player, clients *list)
{

    if (board->board[player->x][player->y] == 111)
    {
        if (input->id == 0)
        {
            board->winner = 1;
        }
        else if (input->id == 1)
        {
            board->winner = 0;
        }
    }
    else if ((board->board[player->x][player->y] != 0 && board->board[player->x][player->y] != '*'))
    {
        if (board->board[player->x][player->y] != list->players[input->id].id)
        {
            if (board->board[player->x][player->y] != list->players[input->id].id + TRAIL_INDEX_SHIFT)
            {
                if (input->id == 0)
                {
                    board->winner = 1;
                }
                else if (input->id == 1)
                {
                    board->winner = 0;
                }
            }
            if (board->board[player->x][player->y] != list->players[input->id].id + TRAIL_INDEX_SHIFT + 30)
            {
                if (input->id == 0)
                {
                    board->winner = 1;
                }
                else if (input->id == 1)
                {
                    board->winner = 0;
                }
            }
        }
    }
}

void lightcycles(display_info *board, struct client_input *input, int index_old, int index_new)
{

    for (int i = 0; i < XMAX; i++)
    {
        for (int j = 0; j < YMAX; j++)
        {
            if (board->board[i][j] == index_old)
            {
                board->board[i][j] = index_new;
            }
        }
    }
}

void upadte_board(display_info *board, struct client_input *input, position *player, clients *list)
{

    position old;
    old.x = player->x;
    old.y = player->y;

    int lock = 0;
    int index = input->id + TRAIL_INDEX_SHIFT + lock;
    if (list->players[input->id].pos.trail == 0)
    {
        lock = 30;
        index = index + lock;
    }
    else if (list->players[input->id].pos.trail == 1)
    {
        index = index - lock;
    }

    switch (input->input)
    {
    case UP:
        old.y--;
        check_winner(board, input, &old, list);
        board->board[player->x][player->y - 1] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->y--;
        //  check_winner(board, input, player, list);

        break;
    case LEFT:
        old.x--;
        check_winner(board, input, &old, list);
        board->board[player->x - 1][player->y] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->x--;
        // check_winner(board, input, player, list);

        break;
    case DOWN:
        old.y++;
        check_winner(board, input, &old, list);
        board->board[player->x][player->y + 1] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->y++;
        // check_winner(board, input, player, list);

        break;
    case RIGHT:
        old.x++;
        check_winner(board, input, &old, list);
        board->board[player->x + 1][player->y] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->x++;
        // check_winner(board, input, player, list);

        break;
    case TRAIL_UP:
        if (list->players[input->id].pos.trail == 0)
        {
            list->players[input->id].pos.trail = 1;
            lightcycles(board, input, index, index + lock);
            index = index + lock;
        }
        else if (list->players[input->id].pos.trail == 1)
        {
            list->players[input->id].pos.trail = 0;
            lightcycles(board, input, index + lock, index);
            index = index - lock;
        }

        break;
        char new_direction = input->input;
        input->input = new_direction;
    }
}

void set(display_info *board)
{
    for (int i = 0; i < XMAX; i++)
    {
        for (int j = 0; j < YMAX; j++)
        {
            if (j == 0 || j == YMAX - 1 || i == 0 || i == XMAX - 1)
            {
                board->board[i][j] = '\0' + 111;
            }
            else
            {
                board->board[i][j] = '*';
            }
        }
    }
}

void set_pos(clients *clients_list)
{

    for (int i = 0; i < clients_list->nb_clients; i++)
    {
        clients_list->players[i].pos.x = rand() % (YMAX - 3) + 3;
        clients_list->players[i].pos.y = rand() % (XMAX - 3) + 3;
        clients_list->players[i].pos.trail = 1;
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

    int refresh_rate = atoi(argv[2]);

    int socket_serv = Server_setup(port);

    while (clients_list->nb_clients < 2)
    {
        init(socket_serv, clients_list);
    }

    int single_terminal = clients_list->players[0].fd;

    if (nb_terminal == 1)
    {
        for (int i = 0; i < clients_list->nb_clients; i++)
        {
            clients_list->players[i].id = i;
            clients_list->players[i].fd = single_terminal;
        }
    }

    struct client_input *player1_input = malloc(sizeof(struct client_input));
    struct client_input *player2_input = malloc(sizeof(struct client_input));
    player1_input->id = 0;
    player2_input->id = 1;
    player1_input->input = LEFT;
    player2_input->input = RIGHT;
    clients_list->players[player1_input->id].pos.trail = 1;
    clients_list->players[player2_input->id].pos.trail = 1;

    printf("lets goooo : \n");
    printf("client %d and socket fd %d\n", clients_list->players[0].id, clients_list->players[0].fd);
    printf("client %d and socket fd %d \n", clients_list->players[1].id, clients_list->players[1].fd);
    printf("server %d\n", socket_serv);

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    display_info data;

    set(&data);

    data.winner = -1;

    // set_pos(clients_list);
    clients_list->players[0].pos.x = 15;
    clients_list->players[0].pos.y = 15;
    clients_list->players[1].pos.x = 10;
    clients_list->players[1].pos.y = 10;

    while (1)
    {
        int maxfd = socket_serv;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(socket_serv, &readfds);
        TV interval;
        interval.tv_sec = 0;
        interval.tv_usec = refresh_rate * 1000;
        for (int i = 0; i < clients_list->nb_clients; i++)
        {
            FD_SET(clients_list->players[i].fd, &readfds);
            if (clients_list->players[i].fd > maxfd)
            {
                maxfd = clients_list->players[i].fd;
            }
        }
        int ret;
        ret = select(maxfd + 1, &readfds, &writefds, &exceptfds, &interval);
        if (ret == -1)
        {
            perror("ERROR : select failed");
            return -1;
        }

        if (ret == 0)
        {

            upadte_board(&data, player1_input, &clients_list->players[player1_input->id].pos, clients_list);
            upadte_board(&data, player2_input, &clients_list->players[player2_input->id].pos, clients_list);

            for (int i = 0; i < clients_list->nb_clients; i++)
            {

                if (send(clients_list->players[i].fd, &data, sizeof(display_info), 0) == -1)
                {
                    perror("ERROR : send failed");
                    return -1;
                }

                printf("refresh\n");
            }
        }
        // interval.tv_sec = 0;
        // interval.tv_usec = refresh_rate * 1000;
        if (ret > 0)
        {
            for (int i = 0; i < clients_list->nb_clients; i++)

            {
                if (FD_ISSET(clients_list->players[i].fd, &readfds))
                {
                    printf(" data at position : %d\n", data.board[clients_list->players[i].pos.x][clients_list->players[i].pos.y]);
                    printf(" recieve from client %d\n", clients_list->players[i].id);
                    struct client_input input;

                    if (recv(clients_list->players[i].fd, &input, sizeof(struct client_input *), 0) == -1)
                    {
                        perror("ERROR : recv failed");
                        return -1;
                    }
                    
                    if (nb_terminal == 1)
                    {
                        
                        if (input.id == player1_input->id)
                        {
                            upadte_board(&data, &input, &clients_list->players[player1_input->id].pos, clients_list);
                            player1_input->input = input.input;
                            clients_list->players[player1_input->id].id = player1_input->id;
                        }
                        else if (input.id == player2_input->id)
                        {
                            upadte_board(&data, &input, &clients_list->players[player2_input->id].pos, clients_list);
                            player2_input->input = input.input;
                            clients_list->players[player2_input->id].id = player2_input->id;
                            }
                     }
                    
                     else if (nb_terminal == 2)
                        {
                        input.id = i;
                            if (input.id == player1_input->id)
                            {
                                upadte_board(&data, &input, &clients_list->players[player1_input->id].pos, clients_list);
                                player1_input->input = input.input;
                                clients_list->players[player1_input->id].id = player1_input->id;
                            }
                            else if (input.id == player2_input->id)
                            {
                                upadte_board(&data, &input, &clients_list->players[player2_input->id].pos, clients_list);
                               
                                player2_input->input = input.input;
                                clients_list->players[player2_input->id].id = player2_input->id;
                            }
                        }

                        printf(" reading and calculating .......");
                        printf(" winner is : %d\n", data.winner);
                        printf("input : %d\n", input.input);
                        printf("input id : %d\n", input.id);
                        printf("sent board to client %d\n", clients_list->players[i].id);
                        printf(" player  position : %d %d %d\n", clients_list->players[i].id, clients_list->players[i].pos.x, clients_list->players[i].pos.y);
                        printf(" data at position : %d\n", data.board[clients_list->players[i].pos.x][clients_list->players[i].pos.y]);
                    }

                    if (send(clients_list->players[i].fd, &data, sizeof(display_info), 0) == -1)
                    {
                        perror("ERROR : send failed");
                        return -1;
                    }
                }
            }
        }
        free(clients_list);
        close(socket_serv);

        return 0;
    }


