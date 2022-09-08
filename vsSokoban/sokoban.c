//
//  sokoban.c
//  sokoban
//
//  Created by Khongmeng Kormoua on 2020. 11. 26..
//  Copyright © 2020. Khongmeng Kormoua. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sokoban.h"


typedef struct {
    char name[30];
    int step;
} player;

// here is all the function definition...
// function to get the position of destination and store them into the array and will check later whether they are done or not yet
int get_destination(char* map) {
    int count = 0;
    int cell;
    
    for (int y = 0; y < MAP_HEIGHT; y += 1)
        for (int x = 0; x < MAP_WIDTH; x += 1) {
            cell = y * MAP_WIDTH + x;
            if (map[cell] == 'x' || map[cell] == '0') {
                destination[count] = cell;
                count += 1;
            }
        }
    return count;
}

// just a simple function to print a map
void print_map(char * board) {
    printf("%s\n", board);
}

// a function to read through the map to find "P", when we found it, store its position, we will use this position later to calculate how to move a character.
void get_position(char* map, int *x, int *y) {
    int cell;
    
    for (int row = 0; row < MAP_HEIGHT; row += 1)
        for (int col = 0; col < MAP_WIDTH; col += 1) {
            cell = row * MAP_WIDTH + col;
            if (map[cell] == 'P') {
                *x = col;
                *y = row;
            }
        }
}

// a function that will handle when the player move a character
void move_char(char* map, int pos_x, int pos_y, int move, int* step) {
    if (map[PLAYER_POSITION + move] != '#') {                   // if the player doesn't hit the wall
         
        if (map[PLAYER_POSITION + move] == 'B' || map[PLAYER_POSITION + move] == '0') {         // if the player pushes box
            map[PLAYER_POSITION] = ' ';                         // replace the current position from 'P' to ' '
            pos_x += move;                                      // update player's position coordinate
            
            if (map[PLAYER_POSITION + move] == ' ') {           // if the box goes to empty space
                map[PLAYER_POSITION + move] = 'B';              // push the box
                map[PLAYER_POSITION] = 'P';                     // move the player position to the new position
            }
            else if (map[PLAYER_POSITION + move] == 'x') {      // if the box goes to the destination
                map[PLAYER_POSITION + move] = '0';              // mark the box as it is in correct place
                map[PLAYER_POSITION] = 'P';                     // move the player position to the new position
            }
            else {                                              // when the box hit to the wall or another box
                map[PLAYER_POSITION - move] = 'P';              // the position of player should be at the previous position (shouldn't move)
                pos_x -= move;                                  // update player's position coordinate back
                *step -= 1;                                     // when the player hit the wall the step shouldn't count
                return;
            }
        }
        else {
            map[PLAYER_POSITION] = ' ';                         // move from the current position
            pos_x += move;                                      // update player's position coordinate
            map[PLAYER_POSITION] = 'P';                         // to the new position
        }
    } else *step -= 1;                                          // if the player hits the wall, the step shouldn't count
}

// a function to open a file, read a map from a file then store a map into a character array
char* get_map(char* map, int choice) {
    char c;
    int lenth = 0;
    char* tmp = NULL;
    FILE *fp = NULL;
    
    switch (choice) {
        case 1:
            fp = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/map00.txt", "r");
            break;
        case 2:
            fp = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/map01.txt", "r");
//            more map is comming
        default:
            break;
    }
    if (fp == NULL) {
        printf("\n\nCouldn't load a map\n\n");
        return NULL;
    }
    while (fscanf(fp, "%c", &c) == 1) {
        tmp = (char*)malloc((lenth + 1 + 1) * sizeof(char));
        for (int i = 0; i < lenth; i++)
            tmp[i] = map[i];
        tmp[lenth] = c;
        tmp[lenth + 1] = '\0';
        lenth++;
        free(map);
        map = tmp;
        tmp = NULL;
    }
    fclose(fp);
    return map;
}

// this function show a main menu and wait for a choice from player
int main_menu(void) {
    int choice;
    printf("\n    Welcome to Sokoban\n\n");
    printf("    Menu:\n");
    printf("        1. Play game\n");
    printf("        2. How to play\n");
    printf("        3. Top 5 players\n");
    printf("        4. Credits\n");
    printf("        5. Exit\n\n\n");
    scanf("%d", &choice);
    return choice;
}

// this function handle when the player have chosen a map and get into a game play
int game_play(char* map) {
    int finish = 0;
    int steps = 0;
    int choice;                                                             // for user to interact after all box are in their place
    char key = '1';                                                         // user input
    int pos_x, pos_y;                                                       // player's coordinates
    int completed_box;                                                      // counter for number of box and dest completed
    int numOf_dest = get_destination(map);                                  // 'x' cells counter
    
    printf("\n    Your steps:\n\n");
    print_map(map);                                                         // print the map
    
    while (key != ESC) {                                                    // game loop, 27 is a integer representation of esc
        get_position(map, &pos_x, &pos_y);                                  // get the position of player
        key = getchar();                                                    // get user input/command
        switch (key) {
            case 'w':
                move_char(map, pos_x, pos_y, UP, &steps);                   // move character up
                steps++;
                break;
            case 's':
                move_char(map, pos_x, pos_y, DOWN, &steps);                 // move character down
                steps++;
                break;
            case 'a':
                move_char(map, pos_x, pos_y, LEFT, &steps);                 // move character to the left
                steps++;
                break;
            case 'd':
                move_char(map, pos_x, pos_y, RIGHT, &steps);                // move character to the right
                steps++;
                break;
            default:
                break;
        }
        completed_box = 0;                                                  // reset a counter for completed box and destination '0'

        for (int i = 0; i < SIZE_OF_DEST; i += 1)                           // go through all array that contain the position destination
            if (map[destination[i]] == '0') completed_box += 1;             // count the completed box if detect in the array
            else if (map[destination[i]] == ' ') map[destination[i]] = 'x'; // it can be ' ' because when we push '0' out it will be ' ', so we need to replace ' ' here by 'x'
        printf("\n    Your steps: %d\n\n", steps);
        print_map(map);
        if (numOf_dest == completed_box) {
            printf("    Congratulation!\n");
            finish = 1;
            key = 27;
        }
    }
    
    // check the best score if the player succeed
    if (finish == 1) {
        printf("    Your steps: %d\n", steps);
        FILE* fpR = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/TopPlayer.txt", "r");
        player p[RECORDED_PLAYER];                                                  // declare player struct to store list of top player
        for (int x = 0; x < RECORDED_PLAYER; x++)                                   // store all top player to the array that we prepare for it
            fscanf(fpR, "%s %d\n", p[x].name, &p[x].step);
        fclose(fpR);
        
        int done = 0;
        for (int x = 0; x < RECORDED_PLAYER && !done; x++)
            if (steps < p[x].step) {                                                // check if the new step is better than any recorded highest score
                char newName[100];
                printf("\n    You are ranked in the top player\n");
                printf("    Please enter your name: ");
                scanf("%s", newName);
                for (int n = 1; n < RECORDED_PLAYER - x; n++) {                     // shift the player down in the list
                    p[RECORDED_PLAYER - n].step = p[RECORDED_PLAYER - n - 1].step;
                    strcpy(p[RECORDED_PLAYER - n].name, p[RECORDED_PLAYER - n - 1].name);
                }
                p[x].step = steps;                                                  // add the new best score to the list
                strcpy(p[x].name, newName);
                printf("\n    Your name has been registered to the Top list!\n");
                done = 1;                                                           // after change the list once then get out of the loop
            }
        FILE* fpW = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/TopPlayer.txt", "w");
        for (int x = 0; x < RECORDED_PLAYER; x++)                                   // store the new list into the file
            fprintf(fpR, "    %s %d\n", p[x].name, p[x].step);
        fclose(fpW);
    } else printf("\n\n    You can always try again.\n");                           // in case of the player can't finish the game
    printf("        0. back\n\n");
    scanf("%d", &choice);                                                           // ask an action from the player
    while (choice != 0) scanf("%d", &choice);
    return choice;
}

// this function will show all the available map to the player and get the choice that player will make
int select_map() {
    int mapth;
    char c;
    FILE *fp = NULL;
    
    printf("\n        1. map1....\n\n");
    fp = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/map00.txt", "r");
    if (fp == NULL) {
        printf("\n\nCouldn't load a map\n\n");
        return EXIT_FAILURE;
    }
    while (fscanf(fp, "%c", &c) == 1)
        printf("%c", c);
    fclose(fp);
    
    printf("\n        2. map2....\n\n");
    fp = fopen("/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/map01.txt", "r");
    if (fp == NULL) {
        printf("\n\nCouldn't load a map\n\n");
        return EXIT_FAILURE;
    }
    while (fscanf(fp, "%c", &c) == 1)
        printf("%c", c);
    fclose(fp);
    
    printf("\n\n\n        0. back\n");
    scanf("%d", &mapth);
    return mapth;
}

// this function read the "HowToPlay.txt" file and print how to play the game to the player
void get_howToPlay(void) {
    char* file = "/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/HowToPlay.txt";
    read_file(file);
}

// this function read the "TopPlayer.txt" file and print the best player so far to the player
void get_topPlayer(void) {
    char* file = "/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/TopPlayer.txt";
    read_file(file);
}

// this function read the "Credits.txt" file and print the credit of creator to the player
void get_Credits(void) {
    char* file = "/Users/meng/Documents/BME/semester3/Programming/final_project/sokoban/sokoban/Credits.txt";
    read_file(file);
}

// this function contain all the mechanism that required to open a file and print all the content in the file to the screen
void read_file(char* file) {
    char c;
    int choice;
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        printf("\n\nCould not open a file\n");
        return;
    }
    printf("\n");
    while (fscanf(fp, "%c", &c) == 1)
        printf("%c", c);
    fclose(fp);
    printf("\n\n\n        0. back\n");
    scanf("%d", &choice);
    while (choice != 0) scanf("%d", &choice);
}
