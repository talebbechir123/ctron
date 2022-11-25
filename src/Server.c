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

void upadte_board(display_info* board, struct client_input* input, position* player)
{
   
    switch (input->input)
    {
    case 'i':
        
        board->board[player->x][player->y - 1] = input->id;
        board->board[player->x][player->y] = input->id+ TRAIL_INDEX_SHIFT;
        player->y--;
        
        break;
    case 'j':
        
        board->board[player->x - 1][player->y] =input->id;
        board->board[player->x][player->y] =  input->id+ TRAIL_INDEX_SHIFT;
         player->x--;

        break;
    case 'k':
         
        board->board[player->x][player->y + 1]  = input->id;
        board->board[player->x][player->y] = input->id+TRAIL_INDEX_SHIFT;
        player->y++;
        // data->board[posx][posy+1]=' ' +  player.id;

        break;
    case 'l':
         
        board->board[player->x + 1][player->y] = input->id;
        board->board[player->x][player->y] =input->id+ TRAIL_INDEX_SHIFT;
        player->x++;
        // data->board[posx+1][posy]= ' ' + player.id;

        break;
    }
}
  

void set( display_info* board)
{
    for(int j=0;j<YMAX;j++){
        for(int i=0;i<XMAX;i++){
            if(j==0 || j==YMAX-1){
                board->board[i][j]=WALL;
            }
             if (i == 0 || i == XMAX-1) {
               
                board->board[i][j]=WALL;
            }
            else{
                board->board[i][j]='\0';
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

    int refresh_rate = atoi(argv[2]);

   
   



    int socket_serv = Server_setup(port);

    while (clients_list->nb_clients < 2)
    {

        init(socket_serv, clients_list);
    }

    printf("lets goooo : \n");
    printf("client %d\n", clients_list->players[0].id);
    printf("client %d\n", clients_list->players[1].id);
    printf("server %d\n", socket_serv);
   
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    // int maxfd = disc_set(socket_serv, &readfds, &writefds, &exceptfds, clients_list);

    position player2;
    
    display_info data;
    set(&data);

   
    data.winner = -1;
    
    //player2.x = 7;
    //player2.y = 8;
  clients_list->players[1].pos.x=17;
  clients_list->players[1].pos.y=18;
  clients_list->players[0].pos.x=7;
  clients_list->players[0].pos.y=8;

   
    int maxfd = socket_serv;

    while (1)
    {

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(socket_serv, &readfds);
         TV interval ;
         interval.tv_sec = 0;
    interval.tv_usec = refresh_rate * 1000;
        for (int i = 0; i < clients_list->nb_clients; i++)
        {
            FD_SET(clients_list->players[i].id, &readfds);
            if (clients_list->players[i].id > maxfd)
            {
                maxfd = clients_list->players[i].id;
            }
        }
        int ret;
         ret=select(maxfd + 1, &readfds, &writefds, &exceptfds, &interval);
        if (ret == -1)
        {
            perror("ERROR : select failed");
            return -1;
        }
        if (ret == 0)
        {
           // printf("timeout\n");
            continue;
        }
        else{
       

        for (int i = 0; i < clients_list->nb_clients; i++)

        {
            if (FD_ISSET(clients_list->players[i].id, &readfds))
            {
                printf(" data at position : %d\n", data.board[clients_list->players[i].pos.x][clients_list->players[i].pos.y]);
                   printf(" recieve from client %d\n",clients_list->players[i].id);
                struct client_input input;
                //int n = read(clients_list->players[i].id, &input, sizeof(struct client_input));
                if(recv(clients_list->players[i].id, &input, sizeof(struct client_input), 0) == -1)
                {
                    perror("ERROR : recv failed");
                    return -1;
                }
                input.id=clients_list->players[i].id;
                upadte_board(&data, &input, &clients_list->players[i].pos);
                
                
                 printf("input : %c\n", input.input);

                if (send(clients_list->players[i].id, &data, sizeof(display_info), 0) == -1)
                {
                    perror("ERROR : send failed");
                    return -1;
                }
                printf("sent board to client %d\n", clients_list->players[i].id);
                printf(" player 2 position : %d %d\n", clients_list->players[i].pos.x, clients_list->players[i].pos.y);
                printf(" data at position : %d\n", data.board[clients_list->players[i].pos.x][clients_list->players[i].pos.y]);
              
                
            }


           
        }
        }
    }
     
    close(socket_serv);
    free(clients_list);

    return 0;
}
