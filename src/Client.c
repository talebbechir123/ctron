#include "header/common.h"
#include "init.c"
#include "con.c"
#include "gameplay.c"
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
#include <sys/select.h>
#include <fcntl.h>
#include <sys/time.h>
#include <ncurses.h>
#include <termios.h>
#include <time.h>



int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Usage: %s [IP_serveur] [port_serveur] [nb_joueurs]", argv[0]);
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int nb_joueurs = atoi(argv[3]);

    int socket_server = 0;

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;

    socket_server = client_setup(ip, port, nb_joueurs, socket_server);
    printf("Client setup done\n");
    display_info display;
    tune_terminal();
    init_graphics();
    srand(time(NULL));
    clear();
    while (1)
    {
        refresh();
        mvaddstr(0, XMAX / 2 - strlen("C-TRON") / 2, "C-TRON");

        int max_fd = socket_server;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        FD_SET(socket_server, &readfds);
        FD_SET(socket_server, &writefds);
        FD_SET(STDIN_FILENO, &readfds);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        if (select(max_fd + 1, &readfds, &writefds, &exceptfds, NULL) < 0)
        {
            perror("ERROR : select failed");
            exit(1);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char c;
           
            read(STDIN_FILENO, &c, 1);
            struct client_input input;
            handle_input(c, &input, nb_joueurs);
            
            

            if (send(socket_server, &input, sizeof(struct client_input), 0) == -1)
            {
                perror("ERROR : send failed");
                exit(1);
            }

            
            if (read(socket_server, &display, sizeof(display_info)) == -1)
            {
                perror("ERROR : read failed");
                exit(1);
            }
             
            
            clear();
            display_game(display);
            refresh();
            if (display.winner != -1)
            {   clear();
                char *winner = malloc(20);
                sprintf(winner, "Le joueur %d a gagné !", display.winner);
                mvaddstr(YMAX / 3,   50 / 2, winner);
                refresh();
                break;
            }
             
           
        }
        if(FD_ISSET(socket_server, &readfds)){
          
            if (read(socket_server, &display, sizeof(display_info)) == -1)
            {
                perror("ERROR : read failed");
                
            }

            
            clear();
            display_game(display);
            refresh();
             if (display.winner != -1)
            {   clear();
                char *winner = malloc(20);
                sprintf(winner, "Le joueur %d a gagné !", display.winner);
                mvaddstr(YMAX / 3,   50 / 2, winner);
                refresh();
                break;
            }
        }
       

        }
        
        close(socket_server);
    }

