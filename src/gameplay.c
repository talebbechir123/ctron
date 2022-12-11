#include "header/gameplay.h"
#include "header/common.h"

/*
 * Function: handle_input
 * --------------------
 * description of what it does : handles input from client
 * if client is player 1 then it handles input for player 1
 * if client is player 2 then it handles input for player 2
 * if nbre of players is 1 then it leaves the input id blank for the server to handle
 * else if nbre of players is 2 then it fills the input id with the values 0 or 1
 *    
 *  Arguments : char c, struct client_input *input, int nbr_joueur
 *
 *  returns: doesn't return anything , sends input to server
 *          
 *       
 */
void handle_input(char c , struct client_input *input, int nbr_joueur ){
 if(nbr_joueur == 1){
     switch (c)
    {
    
    case 'i':
        input->input = UP;
        
        break;
    case 'k':
        input->input = DOWN;
        break;
    case 'j':
        input->input = LEFT;
        break;
    case 'l':
        input->input = RIGHT;
        break;
    case'm' :
        input->input = TRAIL_UP;
        break;
    
    }


 }else if(nbr_joueur == 2){
     switch (c)
    {
    
    case 'i':
        input->input = UP;
        input->id = 0;
        break;
    case 'k':
        input->input = DOWN;
        input->id = 0;
        break;
    case 'j':
        input->input = LEFT;
        input->id = 0;
        break;
    case 'l':
        input->input = RIGHT;
        input->id = 0;
        break;
    case 'm':
        input->input = TRAIL_UP;
        input->id = 0;
        break;
    case 'z':
        input->input = UP;
        input->id = 1;
        break;
    case 's':
        input->input = DOWN;
        input->id = 1;
        break;
    case 'q':
        input->input = LEFT;
        input->id = 1;
        break;
    case 'd':
        input->input = RIGHT;
        input->id = 1;
        break;
    case ' ':
        input->input = TRAIL_UP;
        input->id = 1;
        break;
    }
 }
   
}


/*
 * Function: display_game
 * --------------------
 * description of what it does : displays the game board and the players on it 
 * 
 *    
 *  Arguments : display_info display
 *
 *  returns: doesn't return anything , displays the game board and the players on it
 *          
 *       
 */
void display_game(display_info display){
     for (int i = 0; i < XMAX; i++)
            {
                for (int j = 0; j < YMAX; j++)
                {
                    if(display.board[i][j] == WALL){
                        display_character(CYAN_ON_CYAN, j, i, 'Q');
                    }
                    
                    if (display.board[i][j] == 0)
                    {   
                        display_character(RED_ON_BLACK, j, i, 'X');
                    }
                    if (display.board[i][j] == 1)
                    {   
                        display_character(CYAN_ON_BLACK, j, i, 'X');
                    }
                    if (display.board[i][j] == 50)
                    {   
                        display_character(YELLOW_ON_BLACK + TRAIL_INDEX_SHIFT, j, i, 'X');
                    }
                    if (display.board[i][j] == 51)
                    {    
                        display_character(MAGENTA_ON_BLACK + TRAIL_INDEX_SHIFT, j, i, 'X');
                    }
                }
                 refresh();
            }

}
