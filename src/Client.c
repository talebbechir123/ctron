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

struct client_init_infos  init;



int main(int argc,char *argv[]){
    if(argc != 4){
        printf("Usage: %s [IP_serveur] [port_serveur] [nb_joueurs]", argv[0]);
    }
   char* ip = argv[1];
    int port = atoi(argv[2]);
    int nb_joueurs = atoi(argv[3]);
    init.nb_players=nb_joueurs;
    int socket_client;
    struct sockaddr_in server_addr;

    if((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("ERROR : socket creation failed");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
     inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if(connect(socket_client, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) != 0){
        perror("ERROR : socket connection failed");
        return -1;
    }
    printf("Connection established\n");
    if(send(socket_client, &init, sizeof(struct client_init_infos), 0) == -1){
        perror("ERROR : send failed");
        return -1;
    }
   while(1){
 
   }
    return 0;
}