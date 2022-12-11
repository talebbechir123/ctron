

### func

## SERVER 

int server_send(int client_socket, void *data)

int server_recv(int client_socket, void *data)

void init(int server_socket, clients *list)

int Server_setup(int port)

int disc_set(int socket_serv, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, clients *list)

void check_winner(display_info *board, struct client_input *input, position *player, clients *list)

void lightcycles(display_info *board, struct client_input *input, int index_old, int index_new)

void upadte_board(display_info *board, struct client_input *input, position *player, clients *list)

void set(display_info *board)

void set_pos(clients *clients_list)


## CLIENT

int client_setup(char *ip, int port, int nb_joueurs, int socket_client)

void handle_input(char c , struct client_input *input, int nbr_joueur )

void display_game(display_info display)

