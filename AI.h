#ifndef AI
#define AI
#include "boardRunTime.h"
#include "data.h"

float evaluate(struct player *maximizer, struct player *minimizer)
{
    return maximizer->numberOfStones-minimizer->numberOfStones;
}

struct moveList* generateAllMoves(struct space space, struct player *player)
{
    struct moveList *moveList=NULL;
    for(int level=0;level<space.pileHeight;level++)
        for(int y=0;y<=level;y++)
            for(int x=0;x<=level;x++)
            {
                switch(space.levelSpace[level].levelPlaneFlags[x][y])
                {
                    case 0:
                        break;
                    case WSPARTA:
                        if(space.levelSpace[level].levelPlane[x][y]==PUSTEPOLE) //dolozenie
                        {
                            struct move move;
                            move.heightLand=level;
                            move.xLand=x;
                            move.yLand=y;
                            move.moveType=DOLOZENIE;
                            move.player=player;
                            move.side=player->side;
                            move.stonesAfterMove=player->numberOfStones-1;
                            moveList=addMoveToList(moveList, move);
                        }
                        else if(space.levelSpace[level].levelPlane[x][y]==player->side) //przeniesienie
                        {
                            for(int level2=1;level2<level;level2++)
                                for(int x2=0;x2<=level2;x2++)
                                    for(int y2=0;y2<=level2;y2++)
                                    {
                                        if(space.levelSpace[level2].levelPlaneFlags[x2][y2]==WSPARTA && space.levelSpace[level2].levelPlane[x2][y2]==PUSTEPOLE)
                                        {
                                            struct move move;
                                            move.heightStart=level;
                                            move.xStart=x;
                                            move.yStart=y;
                                            move.heightLand=level2;
                                            move.xLand=x2;
                                            move.yLand=y2;
                                            move.moveType=PRZENIESIENIE;
                                            move.player=player;
                                            move.side=player->side;
                                            move.stonesAfterMove=player->numberOfStones;
                                            if(checkIfLegal(space,move)) moveList=addMoveToList(moveList, move);
                                        }
                                    }
                        }
                        break;
                    case DOZDJECIA:
                        if(space.levelSpace[level].levelPlane[x][y]==player->side)
                        {
                            struct moveList *moveListNew=NULL;
                            for(int xSquare=0;xSquare<=1;xSquare++)
                                for(int ySquare=0;ySquare<=1;ySquare++)
                                {
                                    if(xSquare+x<=level&&ySquare+y<=level&&space.levelSpace[level].levelPlane[x+xSquare][y+ySquare]==player->side && space.levelSpace[level].levelPlaneFlags[x+xSquare][y+ySquare]==DOZDJECIA)
                                    {
                                        if(!(xSquare==0 && ySquare==0))
                                        {
                                            struct move move;
                                            move.heightStart=level;
                                            move.xStart=x;
                                            move.yStart=y;
                                            move.heightLand=level;
                                            move.xLand=x+xSquare;
                                            move.yLand=y+ySquare;
                                            move.moveType=ZDJECIE;
                                            move.player=player;
                                            move.side=player->side;
                                            move.stonesAfterMove=player->numberOfStones+2;
                                            moveListNew=addMoveToList(moveListNew,move);
                                        }
                                        if(xSquare==1 && ySquare==1 && x+1<=level&&space.levelSpace[level].levelPlane[x+1][y]==player->side && space.levelSpace[level].levelPlaneFlags[x+1][y]==DOZDJECIA)
                                        {
                                            struct move move;
                                            move.heightStart=level;
                                            move.xStart=x+1;
                                            move.yStart=y;
                                            move.heightLand=level;
                                            move.xLand=x+xSquare;
                                            move.yLand=y+ySquare;
                                            move.moveType=ZDJECIE;
                                            move.player=player;
                                            move.side=player->side;
                                            move.stonesAfterMove=player->numberOfStones+2;
                                            moveListNew=addMoveToList(moveListNew,move);
                                            if(y+1<=level&&space.levelSpace[level].levelPlane[x][y+1]==player->side && space.levelSpace[level].levelPlaneFlags[x][y+1]==DOZDJECIA)
                                            {
                                                move.xStart=x;
                                                move.yStart=y+1;
                                                moveListNew=addMoveToList(moveListNew,move);
                                            } 
                                        }
                                        struct move move;
                                        move.heightStart=level;
                                        move.xStart=x+xSquare;
                                        move.yStart=y+ySquare;
                                        move.heightLand=-1;
                                        move.moveType=ZDJECIE;
                                        move.player=player;
                                        move.side=player->side;
                                        move.stonesAfterMove=player->numberOfStones+1;
                                        moveListNew=addMoveToList(moveListNew,move);
                                    }
                                    
                                }
                            return moveListNew;
                        }
                        break;
                    default:
                        break;
                }
            }
    return moveList;
}

float minMax(struct space space, struct player *maximizer, struct player *minimizer, int isMaximizing, int depth, int maxDepth, int alpha, int beta, int node)
{
    if(maximizer->numberOfStones==0) return -space.totalNumberOfStones/2;
    if(minimizer->numberOfStones==0) return space.totalNumberOfStones/2;
    if(depth==maxDepth) return evaluate(maximizer, minimizer);

    if(isMaximizing)
    {
        struct moveList *moveList=generateAllMoves(space, maximizer);
        float bestScore=-1000, score;
        while(moveList!=NULL)
        {

            space=makeMove(space, moveList->move);
            if(findFlag(space,DOZDJECIA)) score=minMax(space,maximizer,minimizer,1,depth+1, maxDepth, alpha, beta, node);
            else score=minMax(space,maximizer,minimizer,0,depth+1,maxDepth,alpha,beta, node);
            space=undoMove(space,moveList->move);
            if(score>bestScore) bestScore=score;
            if(alpha<bestScore) alpha=bestScore;
            if(alpha>=beta) break;
            moveList=moveList->nextMove;    
        }
        return bestScore;
    }
    else
    {
        struct moveList *moveList=generateAllMoves(space,minimizer);
        float bestScore=1000,score;
        while(moveList!=NULL)
        {
            space=makeMove(space,moveList->move);
            if(findFlag(space,DOZDJECIA)) score=minMax(space,maximizer,minimizer,0,depth+1, maxDepth,alpha,beta, node);
            else score=minMax(space,maximizer,minimizer,1,depth+1,maxDepth,alpha,beta, node);
            space=undoMove(space,moveList->move);
            if(bestScore>score) bestScore=score;
            if(beta>bestScore) beta=bestScore;
            if(alpha>=beta) break;
            moveList=moveList->nextMove;
        }
        return bestScore;
    }
}

struct move bestMove(struct space space, struct player *maximizer, struct player *minimizer)
{
    struct moveList *moveListHead=generateAllMoves(space, maximizer);
    struct moveList *moveList=moveListHead;
    float bestScore=-1000, score;
    int bestId=0;
    int depth=7;
    if(moveListHead->numberOfMoves<=4) depth=10;
    else if(moveListHead->numberOfMoves<=6) depth=9;
    else if(moveListHead->numberOfMoves<=10) depth=8;
    while(moveList!=NULL)
    {
        int stoneCopy=maximizer->numberOfStones;
        space=makeMove(space,moveList->move);
        if(findFlag(space,DOZDJECIA)) moveList->score=minMax(space,maximizer,minimizer,1,0,depth,-1000,1000, moveList->moveId);
        else moveList->score=minMax(space,maximizer,minimizer,0,0,depth,-1000,1000, moveList->moveId);
        space=undoMove(space,moveList->move);
        if((moveList->score)>bestScore) 
        {
            bestScore=moveList->score;
            bestId=moveList->moveId;
        }
        printf("score: %.2f ID: %d move type: %d ", moveList->score,moveList->moveId,moveList->move.moveType);
        printMove(space,moveList->move,minimizer, maximizer);
        printf("\n");
        moveList=moveList->nextMove;
        
    }
    moveList=moveListHead;
    for(int i=0;i<bestId;i++)
    {
        moveList=moveList->nextMove;
    }
    printf("best score: %.2f ID: %d move type: %d ", moveList->score,moveList->moveId,moveList->move.moveType);
    printMove(space,moveList->move,minimizer, maximizer);
    printf("\n");
    return moveList->move;
}
#endif