#ifndef RUNTIME
#define RUNTIME
#include "data.h"


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

struct move getUserMove(struct player *player, struct space space) //funkcja pobiera ruch od uzytkownika i nie analizuje go
{
    struct move move;
    move.player=player;
    move.side=player->side;
    int levelHeightTemp=100;
    char type[8];

    printf("Rusza sie: %d\nRodzaje ruchu: DOLOZ, PRZENIES, ZDEJMIJ\n: ", player->side);
    scanf("%s",type);
    if(!strcmp(type,"DOLOZ")) move.moveType=DOLOZENIE;
    else if (!strcmp(type,"PRZENIES")) move.moveType=PRZENIESIENIE;
    else if(!strcmp(type,"ZDEJMIJ")) move.moveType=ZDJECIE;
    else return move; //bledny ruch zostanie sprawdzony później funkcją checkIfLegal 
    
    switch(move.moveType)
    {
        case DOLOZENIE:
            printf("\nPozycja na ktorej chcesz polozyc kulke (wysokosc x y)\n: ");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.heightLand=abs(levelHeightTemp-space.pileHeight);
            move.xLand--;
            move.yLand--;
            move.stonesAfterMove=player->numberOfStones-1;
            return move;
        case PRZENIESIENIE:
            printf("\nPozycja z ktorej przenosisz i na ktora przenosisz\nPoczatkowa pozycja (wysokosc x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.heightStart=abs(levelHeightTemp-space.pileHeight);
            printf("\nKoncowa pozycja (wysokość x t)\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.heightLand=abs(levelHeightTemp-space.pileHeight);
            move.xLand--;
            move.yLand--;
            move.xStart--;
            move.yStart--;
            move.stonesAfterMove=player->numberOfStones;
            return move;
        case ZDJECIE:
            printf("\nPozycje dwoch kulek które zdejmujesz\nPierwsza kulka\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xStart,&move.yStart);
            move.heightStart=abs(levelHeightTemp-space.pileHeight);
            printf("\nDruga kulka\n:");
            scanf("%d %d %d", &levelHeightTemp,&move.xLand,&move.yLand);
            move.heightLand=abs(levelHeightTemp-space.pileHeight);
            move.xStart--;
            move.yStart--;
            move.xLand--;
            move.yLand--;
            if(move.heightLand==-1) move.stonesAfterMove=player->numberOfStones+1;
            else move.stonesAfterMove=player->numberOfStones+2;
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
            if(move.heightLand==-1)
            {
                space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
                move.player->numberOfStones++;
            }
                
            else
            {
                space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
                space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
                move.player->numberOfStones+=2;
            }
            break;
        case DOLOZENIE:
            space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=move.side;
            move.player->numberOfStones--;
            break;
        case PRZENIESIENIE:
            space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=PUSTEPOLE;
            space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=move.side;
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
            if(move.heightLand==-1)
            {
                space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=move.side;
                move.player->numberOfStones--;
            }

            else
            {
                space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=move.side;
                space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=move.side;
                move.player->numberOfStones-=2;
            }
            break;
        case DOLOZENIE:
            space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
            move.player->numberOfStones++;
            break;
        case PRZENIESIENIE:
            space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]=move.side;
            space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]=PUSTEPOLE;
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
            if(move.xStart<=move.heightStart&&move.yStart<=move.heightStart&&move.xLand<=move.heightLand&&move.yLand<=move.heightLand&&space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]==move.side && space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]==move.side && space.levelSpace[move.heightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA && space.levelSpace[move.heightLand].levelPlaneFlags[move.xLand][move.yLand]==DOZDJECIA)
                return 1;
            else if(move.xStart<=move.heightStart&&move.yStart<=move.heightStart&&space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]==move.side && move.heightLand==-1 && space.levelSpace[move.heightStart].levelPlaneFlags[move.xStart][move.yStart]==DOZDJECIA)
                return 1;
            else return 0;
        case DOLOZENIE: //dolozenie kulki do planszy
            if(move.xLand<=move.heightLand&&move.yLand<=move.heightLand&&space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.heightLand].levelPlaneFlags[move.xLand][move.yLand]==WSPARTA && move.stonesAfterMove>=0)
                return 1;
            else return 0;
        case PRZENIESIENIE: //przeniesienie kulki wyzej na planszy
            if(move.xStart<=move.heightStart&&move.yStart<=move.heightStart&&move.xLand<=move.heightLand&&move.yLand<=move.heightLand&&move.heightStart>move.heightLand && space.levelSpace[move.heightStart].levelPlane[move.xStart][move.yStart]==move.side && space.levelSpace[move.heightLand].levelPlane[move.xLand][move.yLand]==PUSTEPOLE && space.levelSpace[move.heightStart].levelPlaneFlags[move.xStart][move.yStart]!=ZABLOKOWANA && space.levelSpace[move.heightLand].levelPlaneFlags[move.xLand][move.yLand]==WSPARTA && !((move.xStart-move.xLand)<=1 && (move.yStart-move.yLand)<=1 && (move.xStart-move.xLand)>=0 && (move.yStart-move.yLand)>=0))
            {
                return 1;
            }
                
            else return 0;
        default:
            return 0;
    }
}

void printMove(struct space gameSpace, struct move playerMove, struct player *whitePlayer, struct player *blackPlayer)
{
    switch (playerMove.moveType)
            {
            case ZDJECIE:
                printf("Zdjeto ");
                break;
            case PRZENIESIENIE:
                printf("Przeniesiono %d %d %d %d %d %d ", abs(gameSpace.pileHeight-playerMove.heightStart),playerMove.xStart+1,playerMove.yStart+1,playerMove.heightLand+1,playerMove.xLand+1,playerMove.yLand+1);
                break;
            case DOLOZENIE:
                printf("Dolozono %d %d %d ",abs(gameSpace.pileHeight-playerMove.heightLand),playerMove.xLand+1,playerMove.yLand+1);
                break;
            default:
                break;
            }

}

#endif