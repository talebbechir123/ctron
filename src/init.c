
#include "header/init.h"


 
/*
 * Function: disc_set
 * --------------------
 * description of what it does : sets fd set for select use 
 *    
 *  Arguments : int socket_serv, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, clients *list
 *
 *  returns: max file descriptor
 *          
 *       
 */

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



 
/*
 * Function: set
 * --------------------
 * description of what it does : sets char board and walls
 *    
 *  Arguments : display_info *board
 *
 *  returns: doesn't return anything puts * for empty space and 111 for walls
 *          
 *       
 */
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

/*
 * Function: set_pos
 * --------------------
 * description of what it does : set positions for players 
 *    
 *  Arguments : Clients *clients_list
 *
 *  returns: doesn't return anything sets x and y for each player
 *          
 *       
 */
void set_pos(clients *clients_list)
{

    for (int i = 0; i < clients_list->nb_clients; i++)
    {
        clients_list->players[i].pos.x = rand() % (YMAX - 3) + 3;
        clients_list->players[i].pos.y = rand() % (XMAX - 3) + 3;
        clients_list->players[i].pos.trail = 1;
    }
}