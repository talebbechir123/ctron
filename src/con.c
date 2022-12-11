#include "header/con.h"







/*
 * Function: client_setup
 * --------------------
 * description of what it does : sets up the client
 * 
 *    
 *  Arguments : char *ip, int port, int nb_joueurs, int socket_client
 *
 *  returns: socket_client
 *          
 *       
 */
int client_setup(char *ip, int port, int nb_joueurs, int socket_client)
{
    init_struct.nb_players = nb_joueurs;
    struct sockaddr_in server_addr;

    if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("ERROR : socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);
    if (connect(socket_client, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) != 0)
    {
        perror("ERROR : socket connection failed");
        exit(1);
    }
    printf("Connection established\n");
    if (send(socket_client, &init_struct, sizeof(struct client_init_infos), 0) == -1)
    {
        perror("ERROR : send failed");
        exit(1);
    }
    else
    {
        printf("Send init infos\n");
    }

    return socket_client;
    exit(0);
}


/*
 * Function: Server_setup
 * --------------------
 * description of what it does : sets up the server
 * 
 *    
 *  Arguments : int port
 *
 *  returns: server_socket
 *          
 *       
 */
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



/*
 * Function: init
 * --------------------
 * description of what it does : initializes the clients
 * 
 *    
 *  Arguments : int server_socket, clients *list
 *
 *  returns: void
 *          
 *       
 */
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
