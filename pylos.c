#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STANDARDS
#include "definitions.h"
#include "functionalities.h"



void main()
{   
    struct space gameSpace=createSpace(BOARDSIZE);
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
           // stageprint(gameSpace);
            gameSpace=makeMove(gameSpace,playerMove);
           // stageprint(gameSpace);
            playerTurn.numberOfStones=playerMove.player.numberOfStones;
            whitePlayer=playerTurn;
        }
        else if(playerTurn.side==CZARNAKULKA) //Do manualnego grania gracz vs gracz
        {
            do{
                playerMove=getUserMove(playerTurn,gameSpace);
            }while(!checkIfLegal(gameSpace,playerMove));
           // stageprint(gameSpace);
            gameSpace=makeMove(gameSpace,playerMove);
           // stageprint(gameSpace);
            playerTurn.numberOfStones=playerMove.player.numberOfStones;
            blackPlayer=playerTurn;
        }
        else
        {

            //stageprint(gameSpace);
            playerMove=findBestMove(gameSpace,whitePlayer,blackPlayer);
            makeMove(gameSpace, playerMove);
            //stageprint(gameSpace);
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
                printf("\nPrzeniesiono %d %d %d %d %d %d\n", abs(gameSpace.pileHeight-playerMove.levelHeightStart),playerMove.xStart+1,playerMove.yStart+1,playerMove.levelHeightLand+1,playerMove.xLand+1,playerMove.yLand+1);
                break;
            case DOLOZENIE:
                printf("\nDolozono %d %d %d\n",abs(gameSpace.pileHeight-playerMove.levelHeightLand),playerMove.xLand+1,playerMove.yLand+1);
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