#include "compute.h"
#include "common.h"




//function that deterimens the winner of the game
int winner(display_info* data){
    if(data->winner==-1){
        return -1;
    }
    else if(data->winner==0){
        return 0;
    }
    else if(data->winner==1){
        return 1;
    }
 return 0;
}
//functions that calculate new board after each move
int positon_calcul(display_info* data ,struct client_input player1,struct client_input player2){
return 0;
}


//test
void char_board_gen(display_info* data){
    int i,j;
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
            if(i==0 || i==15){
                data->board[i][j]='1';
            }
            else if(j==0 || j==15){
                data->board[i][j]='1';
            }
            else{
                data->board[i][j]=' ';
            }
            

        }
       
    }
  
}
//test
void board_display(display_info* data){
    
    int i,j;
    for(i=0;i<16;i++){
        for(j=0;j<16;j++){
    printf("%c",data->board[i][j]); 
        }
        printf("\n");
    }
}


int main(int argc, char** argv)
{
    display_info data;
    data.winner=-1;
    char_board_gen(&data);
    board_display(&data);
    return 0;
}