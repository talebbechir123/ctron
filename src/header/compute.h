#include "common.h"



void check_winner(display_info *board, struct client_input *input, position *player, clients *list);

void lightcycles(display_info *board, struct client_input *input, int index_old, int index_new);

void upadte_board(display_info *board, struct client_input *input, position *player, clients *list);
