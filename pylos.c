#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STANDARDS
#include "definitions.h"
#include "functionalities.h"



void main()
{   
    struct space gameSpace=createSpace(4);
    stageprint(stagefill(gameSpace));
    struct player whitePlayer = createPlayer(BIALAKULKA, gameSpace);
    struct player blackPlayer = createPlayer(CZARNAKULKA,gameSpace);
    /*int turn = BIALAKULKA; //przechowuje informację o tym czyja jest teraz tura standardowo zaczynają białe
    while(whitePlayer.numberOfStones!=0 && whitePlayer.numberOfStones!=0) //gra toczy się tak długo jak gracze mają kamienie
    {
        if(1);
    }
    */
}