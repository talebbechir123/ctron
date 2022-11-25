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
#include <ncurses.h>
#include <termios.h>
#include <time.h>
#include "common.h"

#define XMAX 100
#define YMAX 50
#define NB_COLORS 5
#define TRAIL_INDEX_SHIFT 50

#define BLUE_ON_BLACK 0
#define RED_ON_BLACK 2
#define YELLOW_ON_BLACK 1
#define MAGENTA_ON_BLACK 3
#define CYAN_ON_BLACK 4

#define BLUE_ON_BLUE 50
#define RED_ON_RED 52
#define YELLOW_ON_YELLOW 51
#define MAGENTA_ON_MAGENTA 53
#define CYAN_ON_CYAN 54

void tune_terminal()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_iflag &= ~ICANON;
    term.c_lflag &= ~ICANON;
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
}

void init_graphics()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(100);
    start_color();
    init_pair(BLUE_ON_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MAGENTA_ON_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_ON_BLACK, COLOR_CYAN, COLOR_BLACK);

    init_pair(BLUE_ON_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(RED_ON_RED, COLOR_RED, COLOR_RED);
    init_pair(YELLOW_ON_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(MAGENTA_ON_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(CYAN_ON_CYAN, COLOR_CYAN, COLOR_CYAN);

    init_pair(WALL, COLOR_WHITE, COLOR_WHITE);
}

void display_character(int color, int y, int x, char character)
{
    attron(COLOR_PAIR(color));
    mvaddch(y, x, character);
    attroff(COLOR_PAIR(color));
}
int client_build_fdsets(int listenfd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
    int max_fd = listenfd;
    FD_ZERO(readfds);
    FD_SET(listenfd, readfds);
    FD_SET(listenfd, writefds);
    FD_SET(STDIN_FILENO, readfds);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    return max_fd;
}

struct client_init_infos init;

int client_recv(int server_socket, void *data)
{
    int n = 0;
    memset(data, 0, strlen(data));

    if ((n = recv(server_socket, data, sizeof(data), 0)) > 0)
    {
        // process_recv_data(client_socket, recv_msg);
        // printf("Received: %s\n", (void *)data);
    }
    else
    {
        printf("ERROR: recv failed\n");
    }
    return 0;
}

int client_setup(char *ip, int port, int nb_joueurs, int socket_client)
{
    init.nb_players = nb_joueurs;
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
    if (send(socket_client, &init, sizeof(struct client_init_infos), 0) == -1)
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
    while (1)
    {

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
            struct client_input input;

            char c;
            read(STDIN_FILENO, &c, 1);
            input.input = c;
            if (send(socket_server, &input, sizeof(struct client_input), 0) == -1)
            {
                perror("ERROR : send failed");
                exit(1);
            }
            clear();

            if (recv(socket_server, &display, sizeof(display_info), 0) == -1)
            {
                perror("ERROR : recv failed");
                exit(1);
            }
             refresh();
            for (int i = 0; i < YMAX; i++)
            {
                for (int j = 0; j < XMAX; j++)
                {
                    if (i == 0 || i == YMAX - 1)
                    {
                        display_character(CYAN_ON_CYAN, i, j, ACS_VLINE);
                    }
                    else if (j == 0 || j == XMAX - 1)
                    {
                        display_character(CYAN_ON_CYAN, i, j, ACS_HLINE);
                    }
                    if (display.board[j][i] == 4)
                    {
                        display_character(RED_ON_BLACK, i, j, 'X');
                    }
                    if (display.board[j][i] == 5)
                    {
                        display_character(CYAN_ON_BLACK, i, j, 'X');
                    }
                    if (display.board[j][i] == 4 + TRAIL_INDEX_SHIFT)
                    {
                        display_character(YELLOW_ON_BLACK+TRAIL_INDEX_SHIFT, i, j, 'X');
                    }
                    if (display.board[j][i] == 5 + TRAIL_INDEX_SHIFT)
                    {
                        display_character(MAGENTA_ON_BLACK+TRAIL_INDEX_SHIFT, i, j, 'X');
                    }
                }
            }

            refresh();
        }
    }
}
