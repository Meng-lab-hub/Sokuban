//
//  sokoban.h
//  sokoban
//
//  Created by Khongmeng Kormoua on 2020. 11. 26..
//  Copyright © 2020. Khongmeng Kormoua. All rights reserved.
//

#ifndef sokoban_h
#define sokoban_h

#define MAP_WIDTH 14
#define MAP_HEIGHT 10
#define UP - MAP_WIDTH - 1
#define DOWN MAP_WIDTH + 1
#define RIGHT 1
#define LEFT -1
#define PLAYER_POSITION pos_y * MAP_WIDTH + pos_x       // formula to find the index in array of char
#define SIZE_OF_DEST 10                                 // number of possible destination "x" in the map, usually it is at most 10
#define RECORDED_PLAYER 6                               // number of player that will be recorded to the "TopPlayer.txt"
#define ESC 27

// an array that will contain a position of destination
static int destination[SIZE_OF_DEST];

// here is all the function prototype
int get_destination(char*);
char* get_map(char*, int);
void get_position(char*, int *, int *);
void print_map(char*);
int move_char(char*, int, int, int, int*);
int main_menu(void);
int game_play(char*);
int select_map(void);
void get_howToPlay(void);
void get_topPlayer(void);
void get_Credits(void);
void read_file(char*);

#endif /* sokoban_h */
