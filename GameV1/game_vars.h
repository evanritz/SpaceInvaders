#ifndef GAME_VARS_H
#define GAME_VARS_H

#include <stdint.h>

#include "game_consts.h"

typedef enum {dead, alive} status_t;
	
struct sprite{
	int32_t x;	// x coordinate
	int32_t y;	// y coordinate
	int32_t vx, vy;	// velocity. pixels/ 30 Hz
	const unsigned short *image[4];	// ptr -> image. Need 2 images to implement animation for a sprite.
	const unsigned short *black;	// ptr -> black image to cover up the image when sprite is dead.
	status_t life;	// dead/alive
	uint32_t w;	// width
	uint32_t h;	// height
	uint32_t needDraw;	// Semaphore. True if need to draw
	uint32_t hits; 		// holds number of hits val
	uint32_t points;	// holds point val
	uint32_t im_ch; 	// change image semaphore,
};

typedef struct sprite sprite_t;
typedef struct sprite enemy_t;
typedef struct sprite laser_t;
typedef struct sprite missile_t;
typedef struct sprite bunker_t;
typedef struct sprite spaceship_t;

extern int score;
extern int highscore;

extern int lives;

extern int level;

extern int enemy_tally;

extern int i_bound;
extern int j_bound;

extern int game_proc_finished;
extern int game_status;


// enemy sprites
extern enemy_t Enemy[LEVEL_3_ROWS][LEVEL_3_ENEMIES];
extern missile_t Enemy_Missile;
extern laser_t	Enemy_Laser;

// player sprites
extern spaceship_t SpaceShip;
extern laser_t SpaceShip_Laser;
extern bunker_t Bunker[BUNKERS];

#endif
