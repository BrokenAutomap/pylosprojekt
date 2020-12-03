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

struct level createLevel(int levelIndex)  //funkcja tworząca poziom, alokująca pamięć na planszę i flagi do planszy
{
    struct level arg={levelIndex}; 
    arg.levelPlane=calloc((levelIndex+1)*(levelIndex+1),sizeof(int));
    arg.levelPlaneFlags=calloc((levelIndex+1)*(levelIndex+1),sizeof(int));
    return arg;
};

void main()
{

}