// game.c

// Driver of the game
// Holds the control logic for start, gameover, next level of the game
// Written by Evan 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "random.h"
#include "ST7735.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

#include "bitmaps.h"
#include "joystick.h"
#include "hud.h"
#include "Menu.h"

#include "display_consts.h"
#include "game_consts.h"
#include "game_vars.h"
#include "game_task.h"

// used for start menu 
void DelayWait10ms(uint32_t n);

void GameInit(void){
		
	int x, y;
	 
	// create enemies
	y = UPPER_HUD_BOTTOM_RIGHT_Y + ENEMY_H;
	for (int i = 0; i < LEVEL_3_ENEMIES; i++)
	{
		x = UPPER_HUD_TOP_LEFT_X;
		for (int j = 0; j < LEVEL_3_ENEMIES; j++)
		{
			Enemy[i][j].x = x;
			Enemy[i][j].y = y;
			Enemy[i][j].vx = ENEMY_VX;
			Enemy[i][j].vy = ENEMY_VY;
			Enemy[i][j].black = BlackEnemy;
			Enemy[i][j].life = alive;
			Enemy[i][j].w = ENEMY_W;
			Enemy[i][j].h = ENEMY_H;
			Enemy[i][j].needDraw = 1;			
			Enemy[i][j].hits = 0;
			
			Enemy[i][j].image[2] = SmallExplosion0;

			// gen val between 0-2
			int enemy_rand_type = Random32() % 3;
			
			// select enemy types based on rng
			if (enemy_rand_type == 0)
			{
				Enemy[i][j].image[0] = SmallEnemy30pointA;
				Enemy[i][j].image[1] = SmallEnemy30pointB;
				Enemy[i][j].points = 30;
			}
			else if (enemy_rand_type == 1)
			{
				Enemy[i][j].image[0] = SmallEnemy20pointA;
				Enemy[i][j].image[1] = SmallEnemy20pointB; 
				Enemy[i][j].points = 20;
			}
			else if (enemy_rand_type == 2)
			{
				Enemy[i][j].image[0] = SmallEnemy10pointA;
				Enemy[i][j].image[1] = SmallEnemy10pointB;
				Enemy[i][j].points = 10;
			}
			
			x += ENEMY_W + E_S_X;
			
		}
		y += ENEMY_H + E_S_Y;
	}	
	
	// create bunkers
	y = BUNKER_START_Y, x = BUNKER_START_X;
	for (int i = 0; i < BUNKERS; i++)
	{
		Bunker[i].x = x;
		Bunker[i].y = y;
		Bunker[i].vx = 0;
		Bunker[i].vy = 0;
		Bunker[i].black = Bunker3;
		Bunker[i].life = alive;
		Bunker[i].w = BUNKER_W;
		Bunker[i].h = BUNKER_H;
		Bunker[i].image[0] = Bunker0;
		Bunker[i].image[1] = Bunker1;
		Bunker[i].image[2] = Bunker2;
		Bunker[i].needDraw = 1;		
		Bunker[i].hits = 0;		
		x += (MAX_X-BUNKER_W)/BUNKERS;
	}
		
	// Enemy weapons
	// laser
	Enemy_Laser.life = dead;
	Enemy_Laser.vx = 0;
	Enemy_Laser.vy = 1;
	Enemy_Laser.image[0] = Enemy_Laser0;
	//laser.image[1] = ;
	Enemy_Laser.black = Enemy_Laser1;
	Enemy_Laser.w = 2;
	Enemy_Laser.h = 9;
	// missile
	Enemy_Missile.life = dead;
	Enemy_Missile.vx = 0;
	Enemy_Missile.vy = 1;
	Enemy_Missile.image[0] = Missile0;
	Enemy_Missile.image[1] = Missile1;
	Enemy_Missile.image[2] = SmallExplosion0;
	Enemy_Missile.black = Missile2;
	Enemy_Missile.w = 4;
	Enemy_Missile.h = 9;
		
	// Spaceship weapons
	SpaceShip_Laser.life = dead;
	SpaceShip_Laser.vx = 0;
	SpaceShip_Laser.vy = -2;
	SpaceShip_Laser.image[0] = Laser0;
	//laser.image[1] = ;
	SpaceShip_Laser.black = Laser1;
	SpaceShip_Laser.w = 2;
	SpaceShip_Laser.h = 11;

	// Create Spaceship
	SpaceShip.x = SPACESHIP_START_X;
	SpaceShip.y = SPACESHIP_START_Y;
	SpaceShip.vx = 1;
	SpaceShip.vy = 0;	// not moving
	SpaceShip.image[0] = PlayerShip0;
	SpaceShip.image[1] = PlayerShip0;			
	SpaceShip.black = BlackEnemy;
	SpaceShip.life = alive;
	SpaceShip.w = 18;
	SpaceShip.h = 8;
	SpaceShip.needDraw = 1;		
		
		
		
}// GameInit
	
	// used for animation of a sprite.
	unsigned long FrameCount = 0;	
	
	// Draws all game sprites
	void GameDraw(void){
		
		for (int i = 0; i < i_bound; i++)
		{
			for (int j = 0; j < j_bound; j++)
			{
				if(Enemy[i][j].needDraw)
				{
					if(Enemy[i][j].life == alive)
					{
								ST7735_DrawBitmap(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].image[FrameCount], Enemy[i][j].w, Enemy[i][j].h);	// with animation for sprite							
					}
					// if dead
					else
					{
						 if (Enemy[i][j].im_ch)
						 {
								ST7735_DrawBitmap(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].image[2], Enemy[i][j].w, Enemy[i][j].h);	// with animation for sprite
								Enemy[i][j].im_ch = 0;
						 }
						 else
						 {
								ST7735_DrawBitmap(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].black, Enemy[i][j].w, Enemy[i][j].h);					
						 }
					}
					Enemy[i][j].needDraw = 0;	// clear the semaphore				
				}// if
			}
		}
		
		if(SpaceShip_Laser.needDraw)
		{
			if(SpaceShip_Laser.life == alive)
			{
				ST7735_DrawBitmap(SpaceShip_Laser.x, SpaceShip_Laser.y, SpaceShip_Laser.image[0], SpaceShip_Laser.w, SpaceShip_Laser.h);
			}
			else
			{
				ST7735_DrawBitmap(SpaceShip_Laser.x, SpaceShip_Laser.y, SpaceShip_Laser.black, SpaceShip_Laser.w, SpaceShip_Laser.h);					
			}
			SpaceShip_Laser.needDraw = 0;	// clear the semaphore				
		}// if

		
		if(Enemy_Laser.needDraw)
		{
				if(Enemy_Laser.life == alive)
				{
					ST7735_DrawBitmap(Enemy_Laser.x, Enemy_Laser.y, Enemy_Laser.image[0], Enemy_Laser.w, Enemy_Laser.h);
				}
				else
				{
					ST7735_DrawBitmap(Enemy_Laser.x, Enemy_Laser.y, Enemy_Laser.black, Enemy_Laser.w, Enemy_Laser.h);					
				}
				Enemy_Laser.needDraw = 0;	// clear the semaphore				
		}// if
		
		if(Enemy_Missile.needDraw)
		{
				if(Enemy_Missile.life == alive)
				{
					ST7735_DrawBitmap(Enemy_Missile.x, Enemy_Missile.y, Enemy_Missile.image[FrameCount], Enemy_Missile.w, Enemy_Missile.h);
				}
				else
				{
					ST7735_DrawBitmap(Enemy_Missile.x, Enemy_Missile.y, Enemy_Missile.black, Enemy_Missile.w, Enemy_Missile.h);					
				}
				Enemy_Missile.needDraw = 0;	// clear the semaphore				
		}// if
		
		// Draw spaceship
		ST7735_DrawBitmap(SpaceShip.x, SpaceShip.y, SpaceShip.image[FrameCount], SpaceShip.w, SpaceShip.h);	

		
		// Draw bunkers
		for (int i = 0; i < BUNKERS; i++)
		{
			if (Bunker[i].needDraw)
			{
				if (Bunker[i].life == alive)
				{

					if (Bunker[i].im_ch)
					{
						ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].image[Bunker[i].hits], Bunker[i].w, Bunker[i].h);
						Bunker[i].im_ch = 0;
					}
					else
					{
						ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].image[Bunker[i].hits], Bunker[i].w, Bunker[i].h);
					}
				}
				else
				{
					ST7735_DrawBitmap(Bunker[i].x, Bunker[i].y, Bunker[i].black, Bunker[i].w, Bunker[i].h);
					Bunker[i].needDraw = 0;
				}
				//Bunker[i].needDraw = 0;
			}
		}	
		
		
		
		
		FrameCount = (FrameCount+1)&0x01; // 0,1,0,1,...
	}// GameDraw
	

int main(void){
	
	// run mp at 80 MHz
	PLL_Init();
	
	Output_Init();
	
	// create all game sprites
	GameInit();
	
	// init random number gen
	Random_Init(9840423750);
	
	// init joystick adc x and y and digital sw
	Joystick_Init();
	
	// set screen to black
  ST7735_FillScreen(0x0000);	
  
	// load upper hud
	Hud_Menu(score,highscore);
	
	// wait for user input to start game
	while (!Joystick_SW()){
		// print menu start screen
		Menu(Running);
		// wait 300ms
		DelayWait10ms(30);
		// clear menu
		Menu_Clear();
	}
	// clear submenu
	SubMenu_Clear();
	
	// start interrupt
	Game_Task_Init();
	// set game status 
	// Running - 1
	// GameOver - 2
	// NextLevel - 3
	game_status = Running;

	do{
		// interrupt semaphore check
		if (game_proc_finished)
		{

			if (game_status == Running)
			{
				// draw upper and lower hud
				Hud_Ingame(score,highscore,lives,level);
				// draw game sprites
				GameDraw();				
			}
			else if (game_status == GameOver)
			{
				// clear all game sprites
				Menu_Clear();
				SubMenu_Clear();
				Player_Ship_Clear();
				
				// print gameover menu
				Menu(GameOver);	
				
				// wait for user to start game again
				while (!Joystick_SW()){}
				
				// set new highscore
				if (score > highscore)
					highscore = score;
				
				// set game vals back to default
				score = 0;
				level = 1;
				lives = 3;
				
				// clear gameover menu
				Hud_Clear_Upper();
				Menu_Clear();
				SubMenu_Clear();
				
				// recreate all game sprites
				GameInit();
				
				// set game status
				game_status = Running;
					
			}
			else if (game_status == NextLevel)
			{
				
				// inc level
				level++;
				
				// clear all game sprites
				Menu_Clear();
				SubMenu_Clear();
				Player_Ship_Clear();
				
				// recreate all game sprites based off of new level val
				GameInit();
				
				// set game status
				game_status = Running;
			}
			// clear interrupt semaphore
			game_proc_finished = 0;
		}
	}
	while(1);
	
	DelayWait10ms(100);
}//main

void DelayWait10ms(uint32_t n){
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}



