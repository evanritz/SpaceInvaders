// game_vars.c

// Holds game vars for all sprites, indicators, and semaphores
// externs game vars for ease of use

#include "game_vars.h"

// scoring indicators
int score = 0;
int highscore = 0;

// live indicator
int lives = 3;

// level indicator
int level = 1;

// enemy tally indicator
int enemy_tally = 0;

// bound indicators
int i_bound = 0;
int j_bound = 0;

// game semaphores
int Flag;	// Semaphore. Tells foreground to redraw image.
int Anyalive;	// Flag to tell when game is over. When all sprites are dead.

// game semaphores
int game_proc_finished = 0;
int game_status = 0;
//  1 - game running 2 - game over 3- next level

// enemy sprites
// enemy 2D arr
enemy_t Enemy[LEVEL_3_ROWS][LEVEL_3_ENEMIES];
missile_t Enemy_Missile;
laser_t	Enemy_Laser;
	
// player sprites
spaceship_t SpaceShip;
laser_t SpaceShip_Laser;
// bunker 1D arr
bunker_t Bunker[BUNKERS];	

