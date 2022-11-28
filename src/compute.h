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


typedef struct position
{
    int x;
    int y;
    int trail;
} position;

typedef struct player
{
    int id;
    char Keys[4];
    position pos;
} player;
typedef struct clients
{
    player players[MAXCLIENTS];
    int nb_clients;
} clients;


//function that deterimens the winner of the game
int winner(display_info* data);
//functions that calculate new board after each move
int positon_calcul(display_info* data ,struct client_input player);




void set_pos(clients *clients_list){
    int array_x[] ={};
    int array_y[] ={};
    for(int i = 0; i < clients_list->nb_clients; i++){
        int x =rand() % XMAX
        int y =rand() % YMAX 
        if( x == array_x[i] && y == array_y[i]){
            x =rand() % XMAX
            y =rand() % YMAX 
        }
        array_x[i] = x;
        array_y[i] = y;
        clients_list->players[i].pos.x = x;
        clients_list->players[i].pos.y = y;
        clients_list->players[i].pos.trail = 1;
       
    }

    
}



char a[]={'a','5ab','na','8s'};
 for(i = 1; i < (argc); ++i){
    while (n < 4){
        if (argv[i] == a[n]){
            printf("y");
            break;
         }
    }
 }

 void assign_key(clients *clients_list){
    int i = 0;
   
    char a[]={'i','j','k','l','m'};
    char b[]={'z','q','s','d',' '};
    for(i = 0; i < clients_list->nb_clients; i++){
           if(clients_list->players[i].id == 0){
               clients_list->players[i].Keys[0] = 'i';
               clients_list->players[i].Keys[1] = 'j';
               clients_list->players[i].Keys[2] = 'k';
               clients_list->players[i].Keys[3] = 'l';
               clients_list->players[i].Keys[4] = 'm';
               break;
           }
           else if(clients_list->players[i].id == 1){
               clients_list->players[i].Keys[0] = 'z';
               clients_list->players[i].Keys[1] = 'q';
               clients_list->players[i].Keys[2] = 's';
               clients_list->players[i].Keys[3] = 'd';
                clients_list->players[i].Keys[4] = ' ';
               break;
           }
        
    }

    
 }




 int init(clients *clients_list){
    
    return 0;
 }