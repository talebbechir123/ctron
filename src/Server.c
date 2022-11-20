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
#include<sys/select.h>

//nombre de joueurs
 int nb_client = 0;
 //nombre de terminaux connectés
 int nb_terminal=0;
 // tableau de descripteur de client 
int client[2];

int server_send(int client_socket, void* data){
    int n = 0;
    int len  =strlen(data);
    if((n= send(client_socket, data, len, 0)) > 0) {
       
  }  else {
            perror("Error : send failed");
            return -1;
    }

    return n;
}
int server_recv(int client_socket, void *data){
 int n = 0;
    memset(data,0,strlen(data));
    
    if((n = recv(client_socket, data, sizeof(data), 0)) > 0) {
           // process_recv_data(client_socket, recv_msg);
    }
    else {
            printf("ERROR: recv failed\n");
        }
    return 0;
}

int init(int server_socket){
    int socket_client;
    struct sockaddr_in client_addr;
    struct client_init_infos init;
    socklen_t client_addr_len = sizeof(client_addr);
    if((socket_client = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) == -1){
        perror("ERROR : accept failed");
        return -1;
    }
    int n=server_recv(socket_client, &init);
    // int n;
  //  if((n = recv(socket_client, &init, sizeof(struct client_init_infos), 0)) == -1){
     //   perror("ERROR : recv failed");
     //   return -1;
  //  }
    
  nb_client=nb_client + init.nb_players;
   printf("client %d connected\n",nb_client);
 
 
    return socket_client;
}

int Server_setup(int port){
     
    int server_socket;
    struct sockaddr_in server_addr;
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("ERROR : socket creation failed");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1){
        perror("ERROR : socket binding failed");
        return -1;
    }
    if(listen(server_socket, 5) == -1){
        perror("ERROR : listen failed");
        return -1;
    }
    printf("Server listening on port %d", port);
    return server_socket;

}


int disc_set(int socket_serv, fd_set *readfds, fd_set *writefds, fd_set *exceptfds){
    int maxfd = socket_serv;
    FD_ZERO(readfds);
    FD_ZERO(writefds);
    FD_ZERO(exceptfds);
    FD_SET(socket_serv, readfds);
    for(int i = 0; i < nb_terminal; i++){
        FD_SET(client[i], readfds);
        if(client[i] > maxfd){
            maxfd = client[i];
        }
    }
    return maxfd;
}





int main(int argc,char *argv[]){

    if(argc != 3){
        printf("Usage: %s <port> <refresh rate>\n", argv[0]);
        exit(1);
    }
   
    int port = atoi(argv[1]);
    
    int refresh_rate = atoi(argv[2]);
    
    int socket_serv = Server_setup(port);
   
    while(nb_client<2){
        
        int socket_client = init(socket_serv);
        client[nb_terminal]=socket_client;
       client[0]=socket_client;
        nb_terminal++;
    }

     printf("lets goooo : \n");
    printf("client %d\n", client[0]);
    printf("client 2 : %d\n", client[1]);

    fd_set readfds, writefds, exceptfds;
    int maxfd = disc_set(socket_serv, &readfds, &writefds, &exceptfds);
    
  
 
   
 while(1){
     struct client_input input;
     const int result = recv(client[0], &input, sizeof(struct client_input), 0);
    if(result == -1)
    {
        perror("recv");
        break;
    }

    else if(result == 0)
    {
       
        printf( "Client disconnected\n");
        break;
    }

      printf("client %d : sent charrecter %c\n",input.id ,input.input);
 }
        
 
  
 
    close(socket_serv);
      return 0;
}