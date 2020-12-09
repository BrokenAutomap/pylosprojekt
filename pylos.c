#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//pola
#define PUSTEPOLE 0
#define BIALAKULKA 1
#define CZARNAKULKA 2

//flagi
#define DOZDJĘCIA 10 //flaga kulki która nie jest zablokowana i należy do jednej z formacji które można przesunąć/usunąć z planszy np. kwadrat lub linia
#define ZABLOKOWANA 11 //flaga kulki na której leży inna kulka lub nie jest częścią formacji
#define WSPARTA 12 //flaga pustego pola na którym można położyć kulkę (kulka nie może wisieć w powietrzu)

struct space{
    int pileHeight; //wysokość planszy
    int totalNumberOfStones;
    struct level* levelSpace; //wskaźnik do tablicy poziomów
};

struct level{
    int levelIndex; //numer poziomu, (wielkość planszy -1) czyli poziom na samej górze to 0, niżej 1 i tak dalej
    int **levelPlane; //wskaźnik do tablicy przechowującej planszę poziomu
    int **levelPlaneFlags; //wksaźnik do tablicy przechowującej flagi poziomu (gdzie można położyć kulkę, których kulek nie można usunąć bo są przyciśnięte inną kulką wyżej itp. wydaje mi się wstępnie że lepiej będzie to oddzielnie przechowywać niż liczyć przy każdej operacji)
    //proponuję przechowywać dane w formie intów z reprezentacjami liczbowymi zdefiniowanymi w głónym pliku
};

struct player{
    int numberOfStones; //liczba kulek
    int side; //kolor kulek
};

struct level createLevel(int levelIndex)  //funkcja tworząca poziom, alokująca pamięć na planszę i flagi do planszy
{
    struct level createdLevel={levelIndex}; 
    createdLevel.levelPlane = (int **) calloc((levelIndex+1)*(levelIndex+1),sizeof(int*));
        for(int i=0;i<(levelIndex+1)*(levelIndex+1);i++)
            createdLevel.levelPlane[i] = (int *) calloc((levelIndex+1)*(levelIndex+1),sizeof(int)); //dynamiczna tablica 2d pól
    
    createdLevel.levelPlaneFlags = (int **) calloc((levelIndex+1)*(levelIndex+1),sizeof(int*));
        for(int i=0;i<(levelIndex+1)*(levelIndex+1);i++)
            createdLevel.levelPlaneFlags[i] = (int *) calloc((levelIndex+1)*(levelIndex+1),sizeof(int)); //dynamiczna tablica 2d flag
    
    return createdLevel;
}

struct space createSpace(int height) //funkcja tworząca przestrzeń poziomów
{
    struct space createdSpace={height};
    createdSpace.levelSpace=calloc(height,sizeof(struct level));
    for(int i=0;i<height;i++)
    {
        createdSpace.levelSpace[i]=createLevel(i+1); //poziomy ułożone są od góry, poziom o indeksie 0 jest na samej górze i ma wielkość 1   
        createdSpace.totalNumberOfStones+=(i+1)*(i+1); //liczba kamieni jest taka sama jak liczba wszystkich pól
    }
    return createdSpace;
}

struct player createPlayer(int side, struct space wholeBoard)
{
    struct player createdPlayer;
    createdPlayer.side=side;
    createdPlayer.numberOfStones = wholeBoard.totalNumberOfStones/2; //gracz dostaje połowę dostępnych kamieni
    return createdPlayer;
}

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
            board.levelSpace[heightLand].levelPlane[xLand][yLand] = moveMaker.side; //kulka gracza robiącego ruch
        }
    }
    else if(heightLand==-1&&heightStart!=-1) //zdjęcie kulki z planszy
    {
        if(board.levelSpace[heightStart].levelPlane[xStart][yStart] == moveMaker.side && board.levelSpace[heightLand].levelPlaneFlags[xLand][yLand]==DOZDJĘCIA+WSPARTA)
        {
            board.levelSpace[heightStart].levelPlane[xStart][yStart] = PUSTEPOLE;
        }
    }
    else if(heightStart>=0 && heightLand>heightStart) //przeniesienie kulki na planszy, można tylko przenosić do góry
    {
        if(board.levelSpace[heightStart].levelPlane[xStart][yStart] == moveMaker.side && board.levelSpace[heightLand].levelPlaneFlags[xLand][yLand] == WSPARTA)
        {
            board.levelSpace[heightStart].levelPlane[xStart][yStart] = moveMaker.side;
        }
    }
}

void stageprint(struct space space) //można już używać zapisu tab[][], dodałem dynamiczne tablice
{
    for (int i = 0; i < space.pileHeight; i++)
    {
        for (int  row = 0; row <= i; row++)
        {
            for (int col = 0; col <= i; col++)
            {
                printf("%d\t", space.levelSpace->levelPlane[col][row]);
                
            }
            printf("\n");
        }   
        printf("\n\n");
    }  
}

struct space stagefill(struct space space) //można już używać zapisu tab[][], dodałem dynamiczne tablice
{
    struct space newspace = createSpace(space.pileHeight);
    for (int i = 0; i < space.pileHeight; i++)
        for (int  row = 0; row <= i; row++)
            for (int col = 0; col <= i; col++)
            {
                newspace.levelSpace->levelPlane[col][row] = PUSTEPOLE;
                
            } 
    return newspace;
}


void main()
{
    
    
    stageprint(stagefill(createSpace(4)));
}