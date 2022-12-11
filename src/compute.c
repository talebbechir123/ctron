#include "header/compute.h"


 

/*
 * Function: check_winner
 * --------------------
 * description of what it does : checks if there is a winner 
 * 
 *    
 *  Arguments : display_info *board, struct client_input *input, position *player, clients *list
 *
 *  returns: does not return anything , if there is a winner then it sets the winner field in display_info to the id of the winner
 *          
 *       
 */
void check_winner(display_info *board, struct client_input *input, position *player, clients *list)
{

    if (board->board[player->x][player->y] == 111)
    {
        if (input->id == 0)
        {
            board->winner = 1;
        }
        else if (input->id == 1)
        {
            board->winner = 0;
        }
    }
    else if ((board->board[player->x][player->y] != 0 && board->board[player->x][player->y] != '*'))
    {
        if (board->board[player->x][player->y] != list->players[input->id].id)
        {
            if (board->board[player->x][player->y] != list->players[input->id].id + TRAIL_INDEX_SHIFT)
            {
                if (input->id == 0)
                {
                    board->winner = 1;
                }
                else if (input->id == 1)
                {
                    board->winner = 0;
                }
            }
            if (board->board[player->x][player->y] != list->players[input->id].id + TRAIL_INDEX_SHIFT + 30)
            {
                if (input->id == 0)
                {
                    board->winner = 1;
                }
                else if (input->id == 1)
                {
                    board->winner = 0;
                }
            }
        }
    }
}

/*
 * Function: lightcycles
 * --------------------
 * description of what it does : changes the index of the trail of the player to the new index
 * basically it allows for the player previous ligth trail to not be erased by adding a lock value to the index
 * which is usually player id + 50
 *    
 *  Arguments : display_info display
 *
 *  returns: does not return anything , changes the index of the trail of the player to the new index
 *          
 *       
 */
void lightcycles(display_info *board, struct client_input *input, int index_old, int index_new)
{

    for (int i = 0; i < XMAX; i++)
    {
        for (int j = 0; j < YMAX; j++)
        {
            if (board->board[i][j] == index_old)
            {
                board->board[i][j] = index_new;
            }
        }
    }
}
/*
 * Function: upadte_board
 * --------------------
 * description of what it does : updates the board with the new direction of the player and uses 
 * the check_winner function to check if there is a winner and lightcycles to update the trail of the player
 * 
 * 
 * 
 * 
 *    
 *  Arguments : display_info *board, struct client_input *input, position *player, clients *list
 *
 *  returns: does not return anything , updates the board with the new direction of the player
 *          
 *       
 */
void upadte_board(display_info *board, struct client_input *input, position *player, clients *list)
{

    position old;
    old.x = player->x;
    old.y = player->y;

    int lock = 0;
    int index = input->id + TRAIL_INDEX_SHIFT + lock;
    if (list->players[input->id].pos.trail == 0)
    {
        lock = 30;
        index = index + lock;
    }
    else if (list->players[input->id].pos.trail == 1)
    {
        index = index - lock;
    }

    switch (input->input)
    {
    case UP:
        old.y--;
        check_winner(board, input, &old, list);
        board->board[player->x][player->y - 1] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->y--;
        //  check_winner(board, input, player, list);

        break;
    case LEFT:
        old.x--;
        check_winner(board, input, &old, list);
        board->board[player->x - 1][player->y] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->x--;
        // check_winner(board, input, player, list);

        break;
    case DOWN:
        old.y++;
        check_winner(board, input, &old, list);
        board->board[player->x][player->y + 1] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->y++;
        // check_winner(board, input, player, list);

        break;
    case RIGHT:
        old.x++;
        check_winner(board, input, &old, list);
        board->board[player->x + 1][player->y] = input->id;
        board->board[player->x][player->y] = index;
        lightcycles(board, input, index, index);
        player->x++;
        // check_winner(board, input, player, list);

        break;
    case TRAIL_UP:
        if (list->players[input->id].pos.trail == 0)
        {
            list->players[input->id].pos.trail = 1;
            lightcycles(board, input, index, index + lock);
            index = index + lock;
        }
        else if (list->players[input->id].pos.trail == 1)
        {
            list->players[input->id].pos.trail = 0;
            lightcycles(board, input, index + lock, index);
            index = index - lock;
        }

        break;
        char new_direction = input->input;
        input->input = new_direction;
    }
}
