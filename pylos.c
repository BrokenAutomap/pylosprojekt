#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STANDARDS
#include "definitions.h"
#include "functionalities.h"



void main()
{   
    struct space gameSpace=createSpace(4);
    gameSpace=stagefill(gameSpace);
    stageprint(gameSpace);
    struct player whitePlayer = createPlayer(BIALAKULKA, gameSpace);
    struct player blackPlayer = createPlayer(CZARNAKULKA,gameSpace);
    struct move playerMove;
    struct player playerTurn=whitePlayer;
    while(whitePlayer.numberOfStones>0&&blackPlayer.numberOfStones>0)
    {  
        do{
            playerMove=getUserMove(playerTurn,gameSpace);
        }while(!checkIfLegal(gameSpace,playerMove));
        gameSpace=makeMove(gameSpace,playerMove);
        gameSpace=stageflagcheck(gameSpace);
        stageprint(gameSpace);
        if(findFlag(gameSpace,DOZDJECIA)); //jeżeli można zdjąć gracz ma dodatkową turę i nie zmienia się aktywnego gracza
        else 
        {
            if(playerTurn.side==BIALAKULKA) 
            {
                playerTurn=blackPlayer;
            }
            else
            {
                playerTurn=whitePlayer;
            }
        }
    
    }
    
}