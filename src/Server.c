#include "header/common.h"
#include "init.c"
#include "compute.c"
#include "con.c"





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
    
    /* loop to make sure we have 2 clients before starting the game , could 
     be modified to allow more players 
     */
   
    while (clients_list->nb_clients < 2)
    {
        init(socket_serv, clients_list);
    }
    /*
    * if we have 2 players on one terminal we can sets all the client fds to that terminal */
    int single_terminal = clients_list->players[0].fd;
    
    /*
    * setting client list in case of one termnial*/
    if (nb_terminal == 1)
    {
        for (int i = 0; i < clients_list->nb_clients; i++)
        {
            clients_list->players[i].id = i;
            clients_list->players[i].fd = single_terminal;
        }
    }
    /*
    * player set up in case of two players */
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
    printf("set done\n");
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
        //checking for max fd to use in select
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

                
            }
        }
       
        if (ret > 0)
        {
            for (int i = 0; i < clients_list->nb_clients; i++)

            {
                if (FD_ISSET(clients_list->players[i].fd, &readfds))
                {
                    struct client_input input;

                    if (recv(clients_list->players[i].fd, &input, sizeof(struct client_input *), 0) == -1)
                    {
                        perror("ERROR : recv failed");
                        return -1;
                    }
                    //handeling input id and input in case of one terminal
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
                    //handeling input id and input in case of two terminals
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

                    }

                    if (send(clients_list->players[i].fd, &data, sizeof(display_info), 0) == -1)
                    {
                        perror("ERROR : send failed");
                        return -1;
                    }
                }
            }
        }
        char c;
         while(1){
           
           
         read(STDIN_FILENO, &c, 1); 
            if(c == 'q'){
                printf("game over\n");
                return 0;
            }
            else{
                printf("press q to quit\n");
         
         }}


        
        free(clients_list);
        close(socket_serv);

        return 0;
    }


