#ifndef FUNCTIONALITIES
#define FUNCTIONALITIES

#include "definitions.h"
float evaluate(struct player white, struct player black, struct space space, int depth)
{
    float score=0; //dodatni wynik oznacza że czarne mają przewagę
    score = black.numberOfStones - white.numberOfStones;  //gracz mający więcej kamieni w zapasie ma przewagę
    return score;
}

void destroyList(struct moveList* moveList)
{
    if(moveList==NULL) return;
    if(moveList->moveRecord!=NULL)
    {
        destroyList(moveList->moveRecord);
    }
    free(moveList);
    moveList=NULL;
}

int findFlag(struct space space, int value)
{
    for(int level=0;level<space.pileHeight;level++)
        for(int x=0;x<=level;x++)
            for(int y=0;y<=level;y++)
                {
                  if(space.levelSpace[level].levelPlaneFlags[x][y]==value) return 1;  
                }
    return 0;
}

struct space stagefill(struct space space) //można już używać zapisu tab[][], dodałem dynamiczne tablice
{
    struct space newspace = space;
    for (int i = 0; i < space.pileHeight; i++) 
    {
        for (int  row = 0; row <= i; row++)// wszystkie pola ustaw na puste
            for (int col = 0; col <= i; col++)
            {
                newspace.levelSpace[i].levelPlane[col][row] = PUSTEPOLE;
                
            } 
        if (i = space.pileHeight - 1) // wypełnia ostatni lvl wsparciem
        {
            for (int  row = 0; row < space.pileHeight; row++)
                for (int col = 0; col < space.pileHeight; col++)
                {
                    newspace.levelSpace[i].levelPlaneFlags[col][row] = WSPARTA;
                } 
        }
    }
    return newspace;
}

void stageprint(struct space space) // wypisuje zawartość
{
    for (int i = 0; i < space.pileHeight; i++)
    {
        for (int  row = 0; row <= i; row++)
        {
            for (int col = 0; col <= i; col++)
            {
                printf("%d [%d]\t", space.levelSpace[i].levelPlane[col][row], space.levelSpace[i].levelPlaneFlags[col][row]);
            }
            printf("\n");
        }   
        printf("\n");
    }  
}

int kwadrat(struct space space, int col, int row, int level, int zmienna) //sprawdza czy kwadrat kulek jest zapełniony
{
    if (space.levelSpace[level].levelPlane[col][row] != zmienna && space.levelSpace[level].levelPlane[col + 1][row] != zmienna && space.levelSpace[level].levelPlane[col][row + 1] != zmienna && space.levelSpace[level].levelPlane[col + 1][row + 1] != zmienna)
        return 1;
    else
    {
        return 0;
    }
    
}  

int kwadratkolor(struct space space, int col, int row, int level, int zmienna)
{
    int kolor = space.levelSpace[level].levelPlane[col][row];
    int pom = 0;
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            if (space.levelSpace[level].levelPlane[col + j][row + i] != zmienna && space.levelSpace[level].levelPlane[col + j][row + i] == kolor)
                pom += 1;
    
    if (pom == 4)
    {
        return kolor;
    }else
    {
        return 0;
    }
    
}

struct space stageflagcheck(struct space space) // sprawdza flagi 
{
    struct space newspace = space;

    for (int i = 0; i < newspace.pileHeight; i++)// wszystkie na zero
            for (int  row = 0; row <= i; row++)
                for (int col = 0; col <= i; col++)
                    newspace.levelSpace[i].levelPlaneFlags[col][row] = 0;

    for (int  row = 0; row < space.pileHeight; row++) //za każdym razem trzeba ustawiać podłogę jako wspartą, bo flagi na podłodze zmieniają się w trakcie gry, a część znajdująca wspieranie nie uwzględnia podłogi, działa tylko dla wyższych poziomów
        for (int col = 0; col < space.pileHeight; col++)
        {
            newspace.levelSpace[newspace.pileHeight-1].levelPlaneFlags[col][row] = WSPARTA;
        } 

    for (int i = 1; i < newspace.pileHeight; i++)// jeżeli pod polem są kulki to pole jest wsparte
        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
                if (kwadrat(newspace, col, row, i, PUSTEPOLE) == 1)
                    newspace.levelSpace[i - 1].levelPlaneFlags[col][row] = WSPARTA;     

    for (int i = 1; i < newspace.pileHeight; i++)
        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
                if (kwadratkolor(newspace, col, row, i, PUSTEPOLE) == 1 || kwadratkolor(newspace, col, row, i, PUSTEPOLE) == 2) // sprawdza czy kwadrat jest zajęty i czy kolor
                {
                    newspace.levelSpace[i].levelPlaneFlags[col][row] = DOZDJECIA;
                    newspace.levelSpace[i].levelPlaneFlags[col + 1][row + 1] = DOZDJECIA;
                    newspace.levelSpace[i].levelPlaneFlags[col + 1][row] = DOZDJECIA;
                    newspace.levelSpace[i].levelPlaneFlags[col][row + 1] = DOZDJECIA;
                }
    for (int i = 1; i < newspace.pileHeight; i++) // jeżeli na kulkach stoi kulka to znaczy że pod nią kulki są zablokowane
        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
                if(newspace.levelSpace[i - 1].levelPlane[col][row] != PUSTEPOLE)
                {
                    newspace.levelSpace[i].levelPlaneFlags[col][row] = ZABLOKOWANA;
                    newspace.levelSpace[i].levelPlaneFlags[col + 1][row + 1] = ZABLOKOWANA;
                    newspace.levelSpace[i].levelPlaneFlags[col + 1][row] = ZABLOKOWANA;
                    newspace.levelSpace[i].levelPlaneFlags[col][row + 1] = ZABLOKOWANA;
                }


    return newspace;
}

struct move getUserMove(struct player player, struct space space) //funkcja pobiera ruch od uzytkownika i nie analizuje go
{
    struct move move;
    move.player=player;
    int levelHeightTemp=100;
    char type[8];

    printf("Rusza się: %d\nRodzaje ruchu: DOLOZ, PRZENIES, ZDEJMIJ\n: ", player.side);
    scanf("%s",type);
    if(!strcmp(type,"DOLOZ")) move.moveType=DOLOZENIE;
    else if (!strcmp(type,"PRZENIES")) move.moveType=PRZENIESIENIE;
    else if(!strcmp(type,"ZDEJMIJ")) move.moveType=ZDJECIE;
    else return move; //bledny ruch zostanie sprawdzony później funkcją checkIfLegal 
    
    switch(move.moveType)
    {
        case DOLOZENIE:
            printf("\nPozycja na której chcesz położyć kulkę (wysokość x y)\n: ");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.levelHeightLand=abs(levelHeightTemp-space.pileHeight);
            move.xLand--;
            move.yLand--;
            move.player.numberOfStones--;
            return move;
        case PRZENIESIENIE:
            printf("\nPozycja z której przenosisz i na którą przenosisz\nPoczątkowa pozycja (wysokość x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.levelHeightStart=abs(levelHeightTemp-space.pileHeight);
            printf("\nKońcowa pozycja (wysokość x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.levelHeightLand=abs(levelHeightTemp-space.pileHeight);
            move.xLand--;
            move.yLand--;
            move.xStart--;
            move.yStart--;
            return move;
        case ZDJECIE:
            printf("\nPozycje dwóch kulek które zdejmujesz\nPierwsza kulka\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.levelHeightStart=abs(levelHeightTemp-space.pileHeight);
            printf("\nDruga kulka\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.levelHeightLand=abs(levelHeightTemp-space.pileHeight);
            move.xStart--;
            move.yStart--;
            move.xLand--;
            move.yLand--;
            move.player.numberOfStones+=2;
            return move;
        default:
            return move;
    }
}

struct space makeMove(struct space space,struct move move) //funkcja wykonuje dany ruch
{
    switch (move.moveType)
    {
        case ZDJECIE:
            space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
            break;
        case DOLOZENIE:
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=move.player.side;
            break;
        case PRZENIESIENIE:
            space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=move.player.side;
            break;
        default:
            break;
    }
    space=stageflagcheck(space);
    return space;
}

struct space undoMove(struct space space, struct move move)
{
    switch (move.moveType)
    {
        case ZDJECIE:
            if(move.levelHeightLand==-1)
                space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=move.player.side;
            else
            {
                space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=move.player.side;
                space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=move.player.side;
            }
            break;
        case DOLOZENIE:
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
            break;
        case PRZENIESIENIE:
            space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=move.player.side;
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
            break;
        default:
            break;
    }
    space=stageflagcheck(space);
    return space;
}


int checkIfLegal(struct space space, struct move move) //sprawdza czy typ ruchu jest poprawny i czy ruch jest dozwolony
{
    if(findFlag(space,DOZDJECIA) && move.moveType!=ZDJECIE) return 0;
    switch (move.moveType)
    {
        case ZDJECIE: //zdjecie kulek z planszy
            if(space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==move.player.side && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA && space.levelSpace[move.levelHeightLand].levelPlaneFlags[move.xLand][move.yLand]==DOZDJECIA)
                return 1;
            else if(space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && move.levelHeightLand==-1 && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA)
                return 1;
            else return 0;
        case DOLOZENIE: //dolozenie kulki do planszy
            if(space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.levelHeightLand].levelPlaneFlags[move.xLand][move.yLand]==WSPARTA && move.player.numberOfStones>=0)
                return 1;
            else return 0;
        case PRZENIESIENIE: //przeniesienie kulki wyzej na planszy
            if(move.levelHeightStart>move.levelHeightLand && space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]!=ZABLOKOWANA && space.levelSpace[move.levelHeightLand].levelPlaneFlags[move.xLand][move.yLand]==WSPARTA && !((move.xStart-move.xLand)<=1 && (move.yStart-move.yLand)<=1 && (move.xStart-move.xLand)>=0 && (move.yStart-move.yLand)>=0))
            {
                return 1;
            }
                
            else return 0;
        default:
            return 0;
    }
}

struct moveList* addMoveToList(struct moveList *head, struct move move)
{
    if(head==NULL) //jeżeli nie ma głowy, tworzy głowę
    {
        struct moveList *newNode= (struct moveList*) calloc(1,sizeof(struct moveList));
        newNode->moveId=0;
        newNode->moveRecord=NULL;
        newNode->move=move;
        return newNode;

    }
    else //jeżeli jest głowa dodaje do listy
    {
        struct moveList *temp=head;
        struct moveList *newNode = (struct moveList*) calloc(1,sizeof(struct moveList));
        newNode->move=move;
        int moveId=0;
        while(temp->moveRecord!=NULL) 
        {
            temp=temp->moveRecord; //przejście do końca listy
            moveId++;
        }
        if(temp==newNode) return head;
        newNode->moveId=moveId+1;
        newNode->moveRecord=NULL;
        temp->moveRecord=newNode;

        return head;
    }
}

struct moveList* generateAllMoves(struct space space, struct player player)
{
    //przejście po każdym polu na planszy
    struct moveList *possibleMoves=NULL;
    for(int level=0;level<space.pileHeight;level++)
        for(int y=0;y<=level;y++)
            for(int x=0;x<=level;x++)
            {
                switch(space.levelSpace[level].levelPlaneFlags[x][y])
                {
                    case 0: //brak flagi oznacza że nie można nic zrobić z tym polem
                        break;
                    case ZABLOKOWANA: //zablokowanej kulki nie można ruszyć
                        break;
                    case ZDJECIE:  
                        if(space.levelSpace[level].levelPlane[x][y]==player.side)
                        {
                            struct moveList *possibleMoves2=NULL;
                            struct move move;
                            move.moveType=ZDJECIE;
                            move.player.side=player.side;
                            move.player.numberOfStones=player.numberOfStones+1;
                            move.levelHeightStart=level;
                            move.xStart=x;
                            move.yStart=y;
                            move.levelHeightLand=-1;
                            possibleMoves2=addMoveToList(possibleMoves2, move);

                            for(int xHelp=x;xHelp<=x+1;xHelp++) //szukam drugiej kulki aby ją zdjąć
                                for(int yHelp=y;yHelp<=y+1;yHelp++)
                                {   
                                    if(x+xHelp>level||y+yHelp>level) break;
                                    if(space.levelSpace[level].levelPlaneFlags[xHelp][yHelp]==DOZDJECIA && space.levelSpace[level].levelPlane[xHelp][yHelp]==player.side && !(xHelp==x && yHelp==y) )
                                    {
                                        destroyList(possibleMoves); //zdjecie jest jedynym mozliwym ruchem jezeli dostepne
                                        
                                        struct move move;
                                        move.moveType=ZDJECIE;
                                        move.player.side=player.side;
                                        move.player.numberOfStones=player.numberOfStones+2;
                                        move.levelHeightStart=level;
                                        move.xStart=x;
                                        move.yStart=y;
                                        move.levelHeightLand=level;
                                        move.xLand=xHelp;
                                        move.yLand=yHelp;
                                        possibleMoves2=addMoveToList(possibleMoves2, move);
                                    }
                                }
                                return possibleMoves2; //jeżeli można zdjąć jest to jedyny dozwolony ruch    
                        }
                        break;
                    case WSPARTA: //na wspartym polu mozna postawic kulke lub wspartą kulkę przenieść na wsparte pole
                        if(space.levelSpace[level].levelPlane[x][y]==PUSTEPOLE) //dodawanie kulki do planszy
                        {
                            struct move move;
                            move.moveType=DOLOZENIE;
                            move.player.side=player.side;
                            move.player.numberOfStones=player.numberOfStones-1;
                            move.levelHeightLand=level;
                            move.xLand=x;
                            move.yLand=y;
                            possibleMoves=addMoveToList(possibleMoves,move);
                        }
                        else if(space.levelSpace[level].levelPlane[x][y]==player.side) //przenoszenie kulki poziom wyzej //
                        {
                            for(int levelHelp=1;levelHelp<=level;levelHelp++) //szukam miejsc w które mogę przenieść kulkę
                                for(int xHelp=0;xHelp<=levelHelp;xHelp++)
                                    for(int yHelp=0;yHelp<=levelHelp;yHelp++)
                                    {
                                        if(space.levelSpace[levelHelp].levelPlaneFlags[xHelp][yHelp]==WSPARTA)
                                        {
                                            struct move move;
                                            move.moveType=PRZENIESIENIE;
                                            move.player=player;
                                            move.levelHeightStart=level;
                                            move.xStart=x;
                                            move.yStart=y;
                                            move.levelHeightLand=levelHelp;
                                            move.xLand=xHelp;
                                            move.yLand=yHelp;
                                            if(checkIfLegal(space,move))
                                            {
                                                possibleMoves=addMoveToList(possibleMoves, move);
                                            }
                                            else;
                                        }   
                                    } 
                        }
                        break;
                }
            }
    return possibleMoves;
}



float minMax(struct space space, struct player maximizer, struct player minimizer, int isMaximizing, int depth, float alpha, float beta)
{
  
    if(maximizer.numberOfStones==0)
    {
        return -space.totalNumberOfStones/2;
    }
    if(minimizer.numberOfStones==0)
    {
        return space.totalNumberOfStones/2;
    }
    if(depth==MAXDEPTH) 
    { 
        return evaluate(minimizer, maximizer, space, depth);
    }
    if(isMaximizing)
    {
        float bestValue=-1000;
        struct moveList *possibleMovesHead=generateAllMoves(space,maximizer);  
        struct moveList *possibleMoves=possibleMovesHead;
        while(possibleMoves!=NULL)
        {
            space=makeMove(space,possibleMoves->move);
            int stoneHolder=maximizer.numberOfStones;
            maximizer.numberOfStones=possibleMoves->move.player.numberOfStones;
            float value;

            if(findFlag(space,DOZDJECIA)) value=minMax(space,maximizer,minimizer,1,depth+1,alpha,beta);
            else value=minMax(space,maximizer,minimizer,0,depth+1,alpha,beta);
            space=undoMove(space,possibleMoves->move);
            maximizer.numberOfStones=stoneHolder;
            if(bestValue<value) bestValue=value;
            if(alpha<bestValue) alpha=bestValue;
            if(alpha>=beta) break;
            possibleMoves=possibleMoves->moveRecord;
        }
        destroyList(possibleMovesHead);
        return bestValue;
    }
    else
    {
        struct moveList *possibleMovesHead=generateAllMoves(space,minimizer); 
        struct moveList *possibleMoves=possibleMovesHead;
        float bestValue=1000;
        while(possibleMoves!=NULL)
        {
            space=makeMove(space,possibleMoves->move);
            int stoneHolder=minimizer.numberOfStones;
            minimizer.numberOfStones=possibleMoves->move.player.numberOfStones;
            float value;
            if(findFlag(space,DOZDJECIA)) value=minMax(space,maximizer,minimizer,0,depth+1,alpha,beta);
            else  value=minMax(space,maximizer,minimizer,1,depth+1,alpha,beta);

            minimizer.numberOfStones=stoneHolder;
            space=undoMove(space,possibleMoves->move);
            if(bestValue>value) bestValue=value;
            if(beta>bestValue) beta=bestValue;
            if(alpha>=beta) break;
            possibleMoves=possibleMoves->moveRecord;
        }
        destroyList(possibleMovesHead);
        return bestValue;
    }
}

struct move findBestMove(struct space space, struct player white, struct player black)
{
    struct player maximizer=black;
    struct player minimizer=white;
    struct space evaluationSpace;
    evaluationSpace=space;
    struct moveList *moveList=generateAllMoves(evaluationSpace,maximizer);
    struct moveList *currentMove=moveList;
    int bestIndex=0;
    float maxScore=-1000;
    while(currentMove!=NULL)
    {
        evaluationSpace=makeMove(evaluationSpace, currentMove->move);
        int stoneHolder=maximizer.numberOfStones;
        maximizer.numberOfStones=currentMove->move.player.numberOfStones;
        currentMove->move.score=minMax(evaluationSpace, maximizer, minimizer, 0, 1,-1000,1000);
        maximizer.numberOfStones=stoneHolder;
        evaluationSpace=undoMove(evaluationSpace,currentMove->move);
        currentMove=currentMove->moveRecord;
    }
    currentMove=moveList;
    while(currentMove!=NULL)
    {
        if(maxScore<currentMove->move.score)
        {
            maxScore=currentMove->move.score;
            bestIndex=currentMove->moveId;
        }
        printf("%f ID %d move type %d\n", currentMove->move.score,currentMove->moveId,currentMove->move.moveType);
        currentMove=currentMove->moveRecord;
    }
    currentMove=moveList;    
    for(int i=0;i<bestIndex;i++)
    {
        currentMove=currentMove->moveRecord;
    }
    struct move bestMove=currentMove->move;
    //destroyList(moveList);
    printf("najlepszy ruch: %f ID %d\n",bestMove.score,bestIndex);
    return bestMove;

}

#endif