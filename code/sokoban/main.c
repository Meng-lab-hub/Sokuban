//
//  main.c
//  sokoban
//
//  Created by Khongmeng Kormoua on 2020. 10. 27..
//  Copyright © 2020. Khongmeng Kormoua. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sokoban.h"


// a struct of player which will have the name of player and how many step each player takes
typedef struct player{
    char name[30];
    int step;
} player;

//  main function
int main(void) {
    int menu_selection = 0;
    int mapth = 0;                                      // the map that is chosen
    
    while (menu_selection != 5) {
        menu_selection = main_menu();
        if (menu_selection == 1) {
            char* map = (char*)malloc(sizeof(char));
            map[0] = '\0';
            mapth = select_map();
            while (mapth != 0) {
                if (mapth == 1) {
                    map = get_map(map, mapth);
                    if (map != NULL) mapth = game_play(map);
                    else {
                        free(map);
                        return EXIT_FAILURE;
                    }
                }
                if (mapth == 2) {
                    printf("\nSorry, this map will come later.");
                    printf("\n\n\n        0. back.\n");
                    scanf("%d", &mapth);
                    while (mapth != 0) scanf("%d", &mapth);
                }
            }
            free(map);
            map = NULL;
        }
        else if (menu_selection == 2) get_howToPlay();
        else if (menu_selection == 3) get_topPlayer();
        else if (menu_selection == 4) get_Credits();
    }
    printf("    Thank you for Playing!\n");
    return EXIT_SUCCESS;
}
