#include "compute.h"
#include "common.h"


int TRAIL= 1;
int posx;
int posy;

//function that deterimens the winner of the game
int winner(display_info* data){
   
}


int collision(display_info* data,struct client_input player){
    return 0;
}


void Set_trail(int trail){
    if(trail==0){
        TRAIL =0;
    }
}
//functions that calculate new board after each move
int positon_calcul(display_info* data ,struct client_input player){
  
    data->board[posx][posy]=  player.id + TRAIL_INDEX_SHIFT;
  

    switch(player.input){
        case 'i':
       
        data->board[posx][posy-1]=player.id;
            break;
        case 'j':
       
        data->board[posx-1][posy]=player.id;
            break;
        case 'k':
        
        data->board[posx][posy+1]=player.id;
            break;
        case 'l':
        
        data->board[posx+1][posy]=player.id;
            break;
        

    }


return 0;
}



