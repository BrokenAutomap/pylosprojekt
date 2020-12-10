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

struct space makeMove(struct space board, struct player moveMaker,int heightStart, int xStart, int yStart, int heightLand, int xLand, int yLand) //plansza, gracz wykonujący ruch, pozycja startowa, pozycka końcowa
{
//nie zawsze istnieje pozycja końcowa i początkowa (czasami dokłada się kulki z poza planszy lub zdejmuje kulki z planszy, w takim wypadku gdy nieistniejąca pozycja pozątkowa lub końcowa ma zapis height=-1)
    if(heightStart==-1&&heightLand!=-1) //dołożenie kulki do planszy
    {
        if(board.levelSpace[heightLand].levelPlane[xLand][yLand]==PUSTEPOLE&&board.levelSpace[heightLand].levelPlaneFlags[xLand][yLand]==WSPARTA) //kulkę można położyć tylko w miejscu w którym nie ma kulki i gdy miejsce jest wsparte
        {
            board.levelSpace[heightLand].levelPlane[xLand][yLand]=moveMaker.side;
        }
    }
    else if(heightLand==-1&&heightStart!=-1) //zdjęcie kulki z planszy
    {
        if(board.levelSpace[heightStart].levelPlane[xStart][yStart]!=PUSTEPOLE&&board.levelSpace[heightLand].levelPlaneFlags[xLand][yLand]==1);
    }
    else if(heightStart>=0&&heightLand>heightStart) //przeniesienie kulki na planszy, można tylko przenosić do góry
    {
        
    }
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
        for (int  row = 0; row <= i - 1; row++)// 
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

        for (int  row = 0; row <= i - 1; row++)// 
            for (int col = 0; col <= i - 1; col++)
            {
                if (space.levelSpace[i].levelPlane[col][row] != PUSTEPOLE && kwadrat(space, col, row, i, PUSTEPOLE) == 1)
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