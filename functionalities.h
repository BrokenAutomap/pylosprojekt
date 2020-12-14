#ifndef FUNCTIONALITIES
#define FUNCTIONALITIES

#include "definitions.h"

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
    if(type=="DOLOZ") move.moveType=DOLOZENIE;
    else if (type=="PRZENIES") move.moveType=PRZENIESIENIE;
    else if(type=="ZDDEJMIJ") move.moveType=ZDJECIE;
    else return move; //bledny ruch zostanie sprawdzony później funkcją checkIfLegal 
    
    switch(move.moveType)
    {
        case DOLOZENIE:
            printf("\nPozycja na której chcesz położyć kulkę (wysokość x y)\n: ");
            scanf("%d %d %d", levelHeightTemp,move.xLand,move.yLand);
            move.levelHeightLand=abs(levelHeightTemp-space.pileHeight);
            return move;
        case PRZENIESIENIE:
            printf("\nPozycja z której przenosisz i na którą przenosisz\nPoczątkowa pozycja (wysokość x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.levelHeightStart=abs(levelHeightTemp-space.pileHeight);
            printf("\nKońcowa pozycja (wysokość x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.levelHeightLand=abs(levelHeightTemp-space.pileHeight);
            return move;
        case ZDJECIE:
            printf("\nPozycje dwóch kulek które zdejmujesz\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.levelHeightStart=abs(levelHeightTemp-space.pileHeight);
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
    }
return space;
}

int checkIfLegal(struct space space, struct move move) //sprawdza czy typ ruchu jest poprawny i czy ruch jest dozwolony
{
    switch (move.moveType)
    {
        case ZDJECIE: //zdjecie kulki z planszy
            if(space.levelSpace[move.levelHeightStart].levelPlane[move.xStart][move.yStart]==move.player.side && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==move.player.side && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA && space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==DOZDJECIA)
                return 1;
            else return 0;
        case DOLOZENIE: //dolozenie kulki do planszy
            if(space.levelSpace[move.levelHeightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.levelHeightStart].levelPlaneFlags[move.xStart][move.yStart]==WSPARTA)
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

struct move findBestMove(struct moveList moveList, struct space space, struct player white, struct player black)
{

}

struct space stagefill(struct space space) //można już używać zapisu tab[][], dodałem dynamiczne tablice
{
    struct space newspace = createSpace(space.pileHeight);
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
                printf("%d %d\t", space.levelSpace[i].levelPlane[col][row], space.levelSpace[i].levelPlaneFlags[col][row]);
            }
            printf("\n");
        }   
        printf("\n\n");
    }  
}

int kwadrat(struct space space, int col, int row, int level, int zmienna)
{
    if (space.levelSpace[level].levelPlane[col + 1][row] != zmienna && space.levelSpace[level].levelPlane[col][row + 1] != zmienna && space.levelSpace[level].levelPlane[col + 1][row + 1] != zmienna)
        return 1;
    else
    {
        return 0;
    }
    
}  

void stageflagcheck(struct space space) // sprawdza flagi 
{
    for (int i = 1; i < space.pileHeight - 1; i++)
    {

        for (int  row = 0; row <= i - 1; row++)// 
            for (int col = 0; col <= i - 1; col++)
            {
                if (space.levelSpace[i].levelPlane[col][row] != PUSTEPOLE && kwadrat(space, col, row, i, PUSTEPOLE) == 1)
                {
                    space.levelSpace[i -1].levelPlaneFlags[col][row] = WSPARTA;
                }
                
            } 
    } 
    for (int i = 1; i < space.pileHeight - 1; i++)
    {
        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
            {
                if(space.levelSpace[i - 1].levelPlane[col][row] != PUSTEPOLE)
                {
                    space.levelSpace[i].levelPlaneFlags[col][row] = ZABLOKOWANA;
                    space.levelSpace[i].levelPlaneFlags[col + 1][row + 1] = ZABLOKOWANA;
                    space.levelSpace[i].levelPlaneFlags[col + 1][row] = ZABLOKOWANA;
                    space.levelSpace[i].levelPlaneFlags[col][row + 1] = ZABLOKOWANA;
                }
                
            } 
    } 

    for (int i = 1; i < space.pileHeight - 1; i++)
    {

        for (int  row = 0; row <= i - 1; row++)
            for (int col = 0; col <= i - 1; col++)
            {
                if (space.levelSpace[i].levelPlane[col][row] != PUSTEPOLE && kwadrat(space, col, row, i, PUSTEPOLE) == 1) //nie będzie działać, nie uwzględnia koloru kamieni, dowolny kwadrat kulek dostanie taką flagę
                {
                    space.levelSpace[i].levelPlaneFlags[col][row] = DOZDJECIA;
                    space.levelSpace[i].levelPlaneFlags[col + 1][row + 1] = DOZDJECIA;
                    space.levelSpace[i].levelPlaneFlags[col + 1][row] = DOZDJECIA;
                    space.levelSpace[i].levelPlaneFlags[col][row + 1] = DOZDJECIA;
                }
                
            } 
    }
}



#endif