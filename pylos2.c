#include "AI.h"
#include "boardRunTime.h"
#include "data.h"



void main()
{
    struct space space=createSpace(BOARDSIZE);
    struct player white=createPlayer(BIALAKULKA,space);
    struct player black=createPlayer(CZARNAKULKA,space);
    struct player *whitePtr=&white;
    struct player *blackPtr=&black;
    struct move move;
    int playerTurn=BIALAKULKA;
    printf("INSTRUKCJA\nGracz, ktory nie moze wykonac ruchu (brak kulek w zapasie) przegrywa\nMozna dokladac kulki do planszy, przenosic kulki ktore sa juz na planszy poziom wyzej\nlub zdejmowac z planszy kulki ktore tworza formacje 2x2 (kwadrat)\nStan planszy odczytuje sie nastepujaco\nKazde pole ma dwie informacje:\nZ lewej - czy lezy na nim kulka i czyja to kulka\nZ prawej czy mozna polozyc kulke na polu [30], czy mozna ja zdjac [10]\npola zablokowane przez kulke wyzej sa oznaczone jako [20], pole puste na ktorym nie mozna polozyc kulki [0]");
    space=stagefill(space);
    stageprint(space);
    while(whitePtr->numberOfStones!=0&&blackPtr->numberOfStones!=0)
    {
        if(playerTurn==BIALAKULKA)
        {
           do{
                move=getUserMove(whitePtr,space);
            }while(!checkIfLegal(space,move));
            space=makeMove(space,move); 
        }
        else
        {
            move=bestMove(space,blackPtr,whitePtr);
            makeMove(space,move);
        }
        stageprint(space);
        printf("\nGracz 1: %d, gracz 2:%d\n", whitePtr->numberOfStones, blackPtr->numberOfStones);
        printMove(space,move,whitePtr,blackPtr);
        if(findFlag(space,DOZDJECIA));
        else
        {
            if (playerTurn==BIALAKULKA)
            {
                playerTurn=CZARNAKULKA;
            }
            else playerTurn=BIALAKULKA;
            
        }
        
    }
}