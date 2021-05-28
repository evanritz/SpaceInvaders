// game_consts.h

// holds game constants for levels, game status, start postions of sprites

#ifndef GAME_CONSTS_H
#define GAME_CONSTS_H

#include "display_consts.h"

// enemy spacers for x and y
#define E_S_X 3
#define E_S_Y 3

// bunker spacers for x
#define B_S_X 3

#define SPACESHIP_START_Y (LOWER_HUD_TOP_LEFT_Y-3)
#define SPACESHIP_START_X MID_X

#define BUNKER_W 18
#define BUNKER_H 5

#define BUNKERS 3

#define BUNKER_START_X BUNKER_W
#define BUNKER_START_Y (SPACESHIP_START_Y-10)

#define ENEMY_W 16
#define ENEMY_H 10

// game statuses
#define Running 1
#define GameOver 2
#define NextLevel 3

#define ENEMY_VX 1
#define ENEMY_VY 1

// level bounds
#define LEVEL_1_ROWS 1
// Enemies per row
#define LEVEL_1_ENEMIES 6 

#define LEVEL_2_ROWS 2
#define LEVEL_2_ENEMIES 6

#define LEVEL_3_ROWS 3
#define LEVEL_3_ENEMIES 6 

#endif
