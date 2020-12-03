#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//pola
#define PUSTEPOLE 0
#define BIAŁAKULKA 1
#define CZARNAKULKA 2

//flagi
#define DORUSZENIA 10 //flaga kulki która nie jest zablokowana i należy do jednej z formacji które można przesunąć/usunąć z planszy np. kwadrat lub linia
#define ZABLOKOWANA 11 //flaga kulki na której leży inna kulka lub nie jest częścią formacji
#define WSPARTA 12 //flaga pustego pola na którym można położyć kulkę (kulka nie może wisieć w powietrzu)

struct level{
    int levelIndex; //numer poziomu, (wielkość planszy -1) czyli poziom na samej górze to 0, niżej 1 i tak dalej
    int *levelPlane; //wskaźnik do tablicy przechowującej planszę poziomu
    int *levelPlaneFlags; //wksaźnik do tablicy przechowującej flagi poziomu (gdzie można położyć kulkę, których kulek nie można usunąć bo są przyciśnięte inną kulką wyżej itp. wydaje mi się wstępnie że lepiej będzie to oddzielnie przechowywać niż liczyć przy każdej operacji)
    //proponuję przechowywać dane w formie intów z reprezentacjami liczbowymi zdefiniowanymi w głónym pliku
};

struct player{
    int numberOfStones; //liczba kulek
    int side; //kolor kulek
};

struct space{
    int pileHeight; //wysokość planszy
    int totalNumberOfStones;
    struct level* levelSpace; //wskaźnik do tablicy poziomów
};

struct level createLevel(int levelIndex)  //funkcja tworząca poziom, alokująca pamięć na planszę i flagi do planszy
{
    struct level createdLevel={levelIndex}; 
    createdLevel.levelPlane=calloc((levelIndex+1)*(levelIndex+1),sizeof(int));
    createdLevel.levelPlaneFlags=calloc((levelIndex+1)*(levelIndex+1),sizeof(int));
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
    score = black.numberOfStones - white.numberOfStones;
    return score;
}

void main()
{

}