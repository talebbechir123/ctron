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

#define BLUE_ON_BLACK       0
#define RED_ON_BLACK        2
#define YELLOW_ON_BLACK     1
#define MAGENTA_ON_BLACK    3
#define CYAN_ON_BLACK       4

#define BLUE_ON_BLUE        50
#define RED_ON_RED          52
#define YELLOW_ON_YELLOW    51
#define MAGENTA_ON_MAGENTA  53
#define CYAN_ON_CYAN        54


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


void display_character(int color, int y, int x, char character) {
    attron(COLOR_PAIR(color));
    mvaddch(y, x, character);
    attroff(COLOR_PAIR(color));
}


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
      tune_terminal();
    init_graphics();  

    for(;;){
        clear();
        refresh();
        int ch;
        ch = getch();
        struct client_input input;
        input.id=socket_client;
         switch (ch) {
          case 'j': 
            input.input = 'j';
            break;
          case 'k':  
              input.input = 'k';
            break;
          case 'l':  
              input.input = 'l';
            break;
          case 'm':
              input.input = 'm';
            break;
          case 'i':
              input.input = 'i';
            break; 
            case 'w':
                input.input = 'w';
             exit(0);
            break;
        

     }
      
        if(send(socket_client, &input, sizeof(struct client_input), 0) == -1){
            perror("ERROR : send failed");
            return -1;
        }
        refresh();
       
   }
    return 0;
}
}