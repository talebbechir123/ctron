#include "header/echange.h"


/*
 * Function: server_send
 * --------------------
 * description of what it does : sends data to the client
 * 
 *    
 *  Arguments : int client_socket, void *data
 *
 *  returns:  n of bytes sent to the client
 *          
 *       
 */
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
/*
 * Function: server_recv
 * --------------------
 * description of what it does : receives data from the client
 * 
 *    
 *  Arguments : int client_socket, void *data
 *
 *  returns: n of bytes received from the client
 *          
 *       
 */
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
    return n;
}
