#ifndef FUNCTIONALITIES
#define FUNCTIONALITIES

#include "definitions.h"
#include <string.h>
float evaluate(struct player white, struct player black, struct space wholeBoard)
{
    float score=0; //dodatni wynik oznacza że czarne mają przewagę
    score = black.numberOfStones - white.numberOfStones;  //gracz mający więcej kamieni w zapasie ma przewagę
    //   score+= ;  //kamienie w formacji dają małą przewagę
    return score;
}

struct move getUserMove(struct player player, struct space space) //funkcja pobiera ruch od uzytkownika i nie analizuje go
{
    struct move move;
    move.player=player;
    int levelHeightTemp=100;
    char type[8];

    printf("\nRodzaje ruchu: DOLOZ, PRZENIES, ZDEJMIJ\n: ");
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
            printf("\nPozycje dwóch kulek które zdejmujesz\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.levelHeightStart=abs(levelHeightTemp-space.pileHeight);
            move.xStart--;
            move.yStart--;
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
            move.player.numberOfStones+=2;
            break;
        case DOLOZENIE:
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=move.player.side;
            move.player.numberOfStones--;
            break;
        case PRZENIESIENIE:
            space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
            space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]=move.player.side;
            break;
    }
return space;
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

int checkIfLegal(struct space space, struct move move) //sprawdza czy typ ruchu jest poprawny i czy ruch jest dozwolony
{
    if(findFlag(space,DOZDJECIA) && move.moveType!=ZDJECIE) return 0;
    switch (move.moveType)
    {
        case ZDJECIE: //zdjecie kulki z planszy
            if(space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==move.player.side && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==DOZDJECIA)
                return 1;
            else return 0;
        case DOLOZENIE: //dolozenie kulki do planszy
            if(space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.levelHeightLand].levelPlaneFlags[move.xLand][move.yLand]==WSPARTA && move.player.numberOfStones>0)
                return 1;
            else return 0;
        case PRZENIESIENIE: //przeniesienie kulki wyzej na planszy
            if(move.levelHeightStart<move.levelHeightLand && space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]!=ZABLOKOWANA && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==WSPARTA)
                return 1;
            else return 0;
        default:
            return 0;
    }
}

struct move findBestMove(struct moveList historicMoveList, struct space space, struct player white, struct player black)
{
    struct move possibleMovesTable[20];
    int possibleMovesCount=0, depthPossibleMovesCount=0;

    for(int depth=0;depth<10;depth++)
    {

    }



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
        printf("\n\n");
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
    for (int i = 1; i < newspace.pileHeight; i++)// jeżeli pod polem są kulki to pole jest wsparte
        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
                if (kwadrat(newspace, col, row, i, PUSTEPOLE) == 1)
                    newspace.levelSpace[i - 1].levelPlaneFlags[col][row] = WSPARTA;     

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
    return newspace;
}




#endif