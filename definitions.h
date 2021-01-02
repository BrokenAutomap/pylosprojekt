#ifndef DEFINITIONS
#define DEFINITIONS
    #ifndef STANDARDS
    #define STANDARDS
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include <string.h>
    #endif
//pola
#define PUSTEPOLE 0
#define BIALAKULKA 1
#define CZARNAKULKA 2

//flagi
#define DOZDJECIA 10 //flaga kulki która nie jest zablokowana i należy do jednej z formacji które można usunąć z planszy np. kwadrat lub linia
#define ZABLOKOWANA 20 //flaga kulki na której leży inna kulka lub nie jest częścią formacji
#define WSPARTA 30 //flaga pustego pola na którym można położyć kulkę (kulka nie może wisieć w powietrzu)

//rodzaje ruchów
#define DOLOZENIE 1
#define ZDJECIE 2
#define PRZENIESIENIE 3

//maksymalna głębia
#define MAXDEPTH 8


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
    int numberOfStones; //liczba kulek gracza w zapasie
    int side; //kolor kulek
};

struct move{ //struktura do przechowywania pozycji odebranej od użytkownika / ruchu
    int levelHeightStart; //numer poziomu 
    int xStart; //współrzędna x
    int yStart; //współrzędna y
    int levelHeightLand;
    int xLand;
    int yLand;
    int moveType; //rodzaj ruchu
    float score; //ocena ruchu
    struct player player; //gracz wykonujacy ruch
};

struct moveList{ //lista jednokierunkowa przechowująca listę ruchów (potrzebne do cofania ruchów i przeprowadzenia analizy drzewa)
    struct moveList *moveRecord; //wskaźnik do kolejnego ruchu
    
    struct move move; //informacje o ruchu
    int moveId; //numer ruchu, pozycji w liście
};

struct level createLevel(int levelIndex)  //funkcja tworząca poziom, alokująca pamięć na planszę i flagi do planszy
{
    struct level createdLevel={levelIndex}; 
    createdLevel.levelPlane = (int **) calloc((levelIndex+1),sizeof(int*));
        for(int i=0;i<(levelIndex+1);i++)
            createdLevel.levelPlane[i] = (int *) calloc((levelIndex+1),sizeof(int)); //dynamiczna tablica 2d pól
    
    createdLevel.levelPlaneFlags = (int **) calloc((levelIndex+1),sizeof(int*));
        for(int i=0;i<(levelIndex+1);i++)
            createdLevel.levelPlaneFlags[i] = (int *) calloc((levelIndex+1),sizeof(int)); //dynamiczna tablica 2d flag
    
    return createdLevel;
}

struct space createSpace(int height) //funkcja tworząca przestrzeń poziomów
{
    struct space createdSpace={height};
    createdSpace.levelSpace = (struct level*) calloc(height,sizeof(struct level));
    for(int i=0;i<height;i++)
    {
        createdSpace.levelSpace[i]=createLevel(i+1); //poziomy ułożone są od góry, poziom o indeksie 0 jest na samej górze i ma wielkość 1   
        createdSpace.totalNumberOfStones+=(i+1)*(i+1); //liczba kamieni jest taka sama jak liczba wszystkich pól
    }
    return createdSpace;
}

void destroySpace(struct space space)
{
    for(int level=0;level<space.pileHeight;level++)
    {
        for(int col=0;col<=space.levelSpace[level].levelIndex;col++)
        {
            free(space.levelSpace[level].levelPlane[col]);
            free(space.levelSpace[level].levelPlaneFlags[col]);
        }
        free(space.levelSpace[level].levelPlane);
        free(space.levelSpace[level].levelPlaneFlags);
    }
}

struct player createPlayer(int side, struct space wholeBoard)
{
    struct player createdPlayer;
    createdPlayer.side=side;
    createdPlayer.numberOfStones = wholeBoard.totalNumberOfStones/2; //gracz dostaje połowę dostępnych kamieni
    return createdPlayer;
}

#endif