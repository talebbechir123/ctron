#include "compute.h"
#include "common.h"




//function that deterimens the winner of the game
int winner(display_info* data);
//functions that calculate new board after each move
int positon_calcul(display_info* data ,struct client_input player1,struct client_input player2);
