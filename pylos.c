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
    int manual=0;
    while(whitePlayer.numberOfStones>0&&blackPlayer.numberOfStones>0)
    {  
        //printf("manual/auto 1/0\n:");
       // scanf("%d",&manual);
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
       else if(playerTurn.side==CZARNAKULKA && manual) //Do manualnego grania gracz vs gracz
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
        printMove(gameSpace, playerMove, whitePlayer, blackPlayer);
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