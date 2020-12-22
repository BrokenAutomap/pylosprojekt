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
    struct player blackPlayer = createPlayer(CZARNAKULKA, gameSpace);
    struct move playerMove;
    struct player playerTurn=whitePlayer;
    while(whitePlayer.numberOfStones>0&&blackPlayer.numberOfStones>0)
    {  
        if(playerTurn.side==BIALAKULKA)
        {
            do{
                playerMove=getUserMove(playerTurn,gameSpace);
            }while(!checkIfLegal(gameSpace,playerMove));
            stageprint(gameSpace);
            gameSpace=makeMove(gameSpace,playerMove);
            stageprint(gameSpace);
            playerTurn.numberOfStones=playerMove.player.numberOfStones;
            whitePlayer=playerTurn;
        }
        else
        {
            struct space AIspace=createSpace(4);
            AIspace=gameSpace;
            stageprint(gameSpace);
            playerMove=findBestMove(AIspace,whitePlayer,blackPlayer);
            makeMove(gameSpace, playerMove);
            stageprint(gameSpace);
            playerTurn.numberOfStones=playerMove.player.numberOfStones;
            blackPlayer=playerTurn;
        }
        stageprint(gameSpace);
        switch (playerMove.moveType)
            {
            case ZDJECIE:
                printf("\nZdjeto\n");
                break;
            case PRZENIESIENIE:
                printf("\nPrzeniesiono %d %d %d %d %d %d\n", playerMove.levelHeightStart,playerMove.xStart,playerMove.yStart,playerMove.levelHeightLand,playerMove.xLand,playerMove.yLand);
                break;
            case DOLOZENIE:
                printf("\nDolozono\n");
                break;
            default:
                break;
            }
        printf("\nGracz 1: %d, gracz 2:%d\n", whitePlayer.numberOfStones, blackPlayer.numberOfStones);
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