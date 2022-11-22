#include <ncurses.h>

#include <ncurses.h>
#include <termios.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
int posx = 0;
int posy = 0;
typedef struct position
{
    int x;
    int y;
    int id;
    int trail;
} position;



void set_board(display_info *board)
{
    int i, j;
  

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            if (i == 0 || i == 15 || j == 0 || j == 15)
            {
                board->board[i][j] = 111;
            }
            else
            {
                board->board[i][j] = ' ';
            }
        }
    }
}
int check_collision(display_info *board,position player ) {
    
    if(board->board[player.y][player.x] == 111) {
        return 1;
    }
    
     return 0; 
     }
int check_winner(display_info *board, struct client_input input) { 
    
    return 0; 
    }
void upadte_board(display_info *board, struct client_input input) {}

void positon_calcul(display_info *data, struct client_input player)
{

    // sprintf(data->board[posx][posy-1], "%c", player.id);

    switch (player.input)
    {
    case 'i':
        sprintf(&data->board[posx][posy], "%c", player.id + 50);
        sprintf(&data->board[posx][posy - 1], "%c", player.id);
        posy = posy - 1;
        break;
    case 'j':
        sprintf(&data->board[posx][posy], "%c", player.id + 50);
        // data->board[posx-1][posy]= ' ' + player.id;
        sprintf(&data->board[posx - 1][posy], "%c", player.id);
        posx = posx - 1;
        break;
    case 'k':

        sprintf(&data->board[posx][posy], "%c", player.id + 50);
        sprintf(&data->board[posx][posy + 1], "%c", player.id);
        // data->board[posx][posy+1]=' ' +  player.id;
        posy = posy + 1;
        break;
    case 'l':
        sprintf(&data->board[posx][posy], "%c", player.id + 50);
        sprintf(&data->board[posx + 1][posy], "%c", player.id);
        // data->board[posx+1][posy]= ' ' + player.id;
        posx = posx + 1;
        break;
    }
}


// test
void board_display(display_info *data)
{

    int i, j;
    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++)
        {
            printf("%c", data->board[i][j]);
        }
        printf("\n");
    }
}

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

int main(int argc, char **argv)
{

    // tune_terminal();
    // init_graphics();
    // srand(time(NULL));
    display_info *data = malloc(sizeof(display_info));
    struct client_input player;
    position player1;
    position player2;
    position player3;

    posx = 2;
    posy = 2;
    data->winner = -1;
    // char_board_gen(&data);
    set_board(data);
    player1.x = 0;
    player1.y = 0;
    player2.x = 15;
    player2.y = 15;
    player3.x=8;
    player3.y=9;
    
    board_display(data);
    //printf("%d\n", data->board[8][9]);
    printf(" p1 collison : %d\n", check_collision(data,player1));
     printf(" p2 collison : %d\n", check_collision(data,player2));
       printf(" p3 collison : %d \n", check_collision(data,player3));
   

    // convert 123 to string [buf]
 

    // print our string
   // printf("%s\n", snum);
    // player = (struct client_input) {50, 'l'};

    // positon_calcul(&data,player);
    // board_display(&data);
    // player = (struct client_input) {2, 'l'};
    // positon_calcul(&data,player);
    // board_display(&data);

    // for(;;){
    //  clear();
    //  refresh();
    //  int ch;

    // ch = getch();

    // if(ch == 'g'){ exit(0);}

    // }

    // for (;;)
    // {
    //     clear();
    //     for (size_t i = 0; i < YMAX; i++)
    //     {
    //         for (size_t j = 0; j < XMAX; j++)
    //         {
    //             if (i == 0 || i == YMAX - 1)
    //             {
    //                 display_character(CYAN_ON_CYAN, i, j, ACS_VLINE);
    //             }
    //             else if (j == 0 || j == XMAX - 1)
    //             {
    //                 display_character(CYAN_ON_CYAN, i, j, ACS_HLINE);
    //             }
    //             display_character(BLUE_ON_BLACK, 8, 5, data.board[8][5]);
    //             display_character(BLUE_ON_BLACK + TRAIL_INDEX_SHIFT, 8, 5, data.board[8][5]);
    //         }
    //         mvaddstr(0, XMAX / 2 - strlen("C-TRON") / 2, "C-TRON");
    //         mvaddstr(YMAX / 2, XMAX / 2, "TESTING");
    //     }

    //     sleep(2);
    //     refresh();
    // }

    return 0;
}
