// game_task.c

// ISR Game Task to check controls, move and generate temp sprites (weapons) and preform hit detection

#include "game_task.h"

#include "tm4c123gh6pm.h"
#include "joystick.h"
#include "game_vars.h"
#include "random.h"

// Game Task Systick delay for 30Hz, might want to uncomment below if sprite overlap happens on higher levels
unsigned long FR_30_HZ = 2664000+1000000;
//unsigned long FR_30_HZ = 2664000+750000;
//unsigned long FR_30_HZ = 2664000;
unsigned long FR_60_HZ = 1332000;

// Systick Interrupt Init at 30Hz
void Game_Task_Init(void)
{
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = FR_30_HZ-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
  EnableInterrupts();	// make I = 0
}

void SysTick_Handler(void)
{
	
	// --- GAME TASK --- 
	
	// get current inputs
	Joystick_Update();
	
	// select correct level bounds
	Level_Size();
	
	// tally dead enemies
	Enemy_Tally();
	
	// update sprites postions
	Game_Move();

	// generate weapons if needed
	Shoot_Enemy_Weapons();
	
	// preform hit detection
	Hit_Detection();
	
	
	game_proc_finished = 1;
	
}

void Game_Move(void)
{
	
	
	// move player ship based on input
	int X_Direction = Joystick_X();
	
	if (X_Direction == Left && SpaceShip.x > MIN_X+1)
	{
		SpaceShip.x -= SpaceShip.vx;
	}
	else if (X_Direction == Right && SpaceShip.x < MAX_X-1-SpaceShip.w)
	{
		SpaceShip.x += SpaceShip.vx;
	}
	
	// shoot laser based on input
	bool SW_Pressed = Joystick_SW();
	
	if (SW_Pressed && SpaceShip_Laser.life == dead)
	{
		Shoot_SpaceShip_Laser();
	}
	
	// move spaceship laser
	if(SpaceShip_Laser.life == alive)
	{
		SpaceShip_Laser.needDraw = 1;

		if(SpaceShip_Laser.y < UPPER_HUD_BOTTOM_RIGHT_Y + SpaceShip_Laser.h)
			SpaceShip_Laser.life = dead;
		
		SpaceShip_Laser.y += SpaceShip_Laser.vy;
	}
		
	// move enemy laser
	if (Enemy_Laser.life == alive)
	{
		Enemy_Laser.needDraw = 1;
		
		if (Enemy_Laser.y > LOWER_HUD_BOTTOM_RIGHT_Y + Enemy_Laser.h)
			Enemy_Laser.life = dead;
		
		Enemy_Laser.y += Enemy_Laser.vy;
		
	}
		
	// move enemy missile
	if (Enemy_Missile.life == alive)
	{
		Enemy_Missile.needDraw = 1;
		
		if (Enemy_Missile.y > LOWER_HUD_BOTTOM_RIGHT_Y + Enemy_Missile.h)
			Enemy_Missile.life = dead;
		
		Enemy_Missile.y += Enemy_Missile.vy;
		
	}
		
	// move enemies
	for (int i = 0; i < i_bound; i++)
	{
		
		for (int j = 0; j < j_bound; j++)
		{
			
				// Since movement of enemy sprites is tied to 2D arr bounds, it must move dead enemies sprites (black) to stay in order
				Enemy[i][j].needDraw = 1;
				
				// if enemies make to the bunkers, set game status semaphore to gameover
				if((Enemy[i][j].y > BUNKER_START_Y-BUNKER_H) || (Enemy[i][j].y < UPPER_HUD_TOP_LEFT_Y)){
					game_status = GameOver;
				}		
				
				// moves enemies back and forth when 1st and last most hit edges
				// moves enemies down too
				if((Enemy[i][j].x < MIN_X) || (Enemy[i][j].x > MAX_X-Enemy[i][j].w)){

					for (int p = 0; p < j_bound; p++)
					{
						Enemy[i][p].y += Enemy[i][p].vy;
						Enemy[i][p].vx = -1*Enemy[i][p].vx;
					}
					
					if (j == j_bound-1)
					{
							for (int p = 0; p < j_bound-1; p++)
							{
								Enemy[i][p].x += 2*Enemy[i][p].vx;
							}
					}
				}
				Enemy[i][j].x += Enemy[i][j].vx;	
		}
	}
	
}
void Shoot_SpaceShip_Laser(void)
{
	// update laser to current spaceship postion 
	SpaceShip_Laser.x = SpaceShip.x + SpaceShip.w/2;
	SpaceShip_Laser.y = SpaceShip.y;
	SpaceShip_Laser.life = alive;
	SpaceShip_Laser.needDraw = 1;
}

void Level_Size(void)
{
	// based on level indicator, set 2D arr bounds accordingly
	switch (level)
		{
			case 1:
				i_bound = LEVEL_1_ROWS;
				j_bound = LEVEL_1_ENEMIES;
				break;
			case 2:
				i_bound = LEVEL_2_ROWS;
				j_bound = LEVEL_2_ENEMIES;
				break;
			default:
				i_bound = LEVEL_3_ROWS;
				j_bound = LEVEL_3_ENEMIES;
				break;
		}
}

// counts number of dead enemies, if level amt is reached, set game status semaphore to nextlevel
void Enemy_Tally(void)
{
	enemy_tally = 0;
	for (int i = 0; i < i_bound; i++)
		for(int j = 0; j < j_bound; j++)
			if (Enemy[i][j].life == dead)
				enemy_tally++;
			
	if (enemy_tally == i_bound*j_bound)
	{
		game_status = NextLevel;
	}
}

// uses rng to spawn enemy weapons on highest row (lowest to user) on random col
void Shoot_Enemy_Weapons(void)
{ 
	
	if (Enemy_Laser.life == dead || Enemy_Missile.life == dead)
	{
		int i = i_bound-1;
		int j = Random32() % j_bound; // random enemy on last row
		
		// find a alive enemy in col
		while (Enemy[i][j].life != alive)
		{
			if (i > 0)
				i--;
			else
			{
				i = i_bound-1;
				j = Random32() % j_bound;
			}
		}
		// center weapons in bottom middle of enemy
		int weapon_x = Enemy[i][j].x + ENEMY_W/2;
		int weapon_y = Enemy[i][j].y + ENEMY_H;
		
		// spawn weapons 
		if (Enemy_Laser.life == dead)
		{
			Enemy_Laser.x = weapon_x;
			Enemy_Laser.y = weapon_y;
			Enemy_Laser.life = alive;
			Enemy_Laser.needDraw = 1;
		}	
		else if (Enemy_Missile.life == dead)
		{
			Enemy_Missile.x = weapon_x;
			Enemy_Missile.y = weapon_y;
			Enemy_Missile.life = alive;
			Enemy_Missile.needDraw = 1;
		}	
	}
}

void Hit_Detection(void)
{
	
	// hit detect for spaceship laser hitting enemies and ceiling
	if (SpaceShip_Laser.life == alive)
	{
		int spl_x0 = SpaceShip_Laser.x;
		int spl_y0 = SpaceShip_Laser.y;
		
		int spl_w = SpaceShip_Laser.w;
		int spl_h = SpaceShip_Laser.h;
		
		int spl_x1 = spl_x0 + spl_w;
		int spl_y1 = spl_y0 + spl_h;
		
		
		for (int i = 0; i < i_bound; i++)
		{
			for (int j = 0; j < j_bound; j++) 
			{
				enemy_t n_enemy = Enemy[i][j];
				
				int e_x0 = n_enemy.x;
				int e_y0 = n_enemy.y;
				
				int e_w  = n_enemy.w;
				int e_h  = n_enemy.h;
				
				int e_x1 = e_x0 + e_w;
				int e_y1 = e_y0 + e_h;
				
				// with the x range of the enemy
				if ((e_x0 <= spl_x0) && (e_x1 >= spl_x1 ))
				{
					// at or above the y vaule of the enemy
					if (e_y1 >= spl_y0)
					{
						if (Enemy[i][j].hits < 1)
						{
							// add point to score
							score += Enemy[i][j].points;
							// kill player laser and enemy
							SpaceShip_Laser.life = dead;
							Enemy[i][j].life = dead;
							
							Enemy[i][j].hits++;
							// change to explosion 
							Enemy[i][j].im_ch = 1;
						}
					}
				}
			}
		}
		
		// ceiling check
		if (spl_y0 <= UPPER_HUD_BOTTOM_RIGHT_Y)
		{
			SpaceShip_Laser.life = dead;
		}
		
	}
	
	// hit detect for enemy weapons hitting bunkers, spaceship, and floor
	if (Enemy_Laser.life == alive || Enemy_Missile.life == alive)
	{
		
		int laser_x0, laser_y0, laser_x1, laser_y1;
		int missile_x0, missile_y0, missile_x1, missile_y1;
		
		
		if (Enemy_Laser.life == alive)
		{
			laser_x0 = Enemy_Laser.x;
			laser_y0 = Enemy_Laser.y;
			laser_x1 = laser_x0 + Enemy_Laser.w;
			laser_y1 = laser_y0 + Enemy_Laser.h;
		}
		
		if (Enemy_Missile.life == alive)
		{
			missile_x0 = Enemy_Missile.x;
			missile_y0 = Enemy_Missile.y;
			missile_x1 = missile_x0 + Enemy_Missile.w;
			missile_y1 = missile_y0 + Enemy_Missile.h;
		}
		
		// hit detect for enemy laser hitting bunkers
		for (int b = 0; b < BUNKERS; b++)
		{
			bunker_t bunker = Bunker[b];
			if (bunker.life == alive)
			{
				if (((bunker.x <= laser_x0) && (bunker.x+bunker.w >= laser_x1)))
				{
					if ((bunker.y+bunker.h <= laser_y1))
					{
						if (Bunker[b].hits < 2)
						{
								// kill enemy laser
								Enemy_Laser.life = dead;
								// inc hits
								Bunker[b].hits++;
								// change to bunker damage based on hits
								Bunker[b].im_ch = 1;
						}
						else
						{
							Bunker[b].life = dead;
						}
					}
				}
				
			}
		}
		
		// hit detect for enemy missile hitting bunkers
		for (int b = 0; b < BUNKERS; b++)
		{
			bunker_t bunker = Bunker[b];
			if (bunker.life == alive)
			{
				if (((bunker.x <= missile_x0) && (bunker.x+bunker.w >= missile_x1)))
				{
					if ((bunker.y+bunker.h <= missile_y1))
					{
						if (Bunker[b].hits < 2)
						{
								Enemy_Missile.life = dead;
								Bunker[b].hits++;
								Bunker[b].im_ch = 1;
						}
						else
						{
							Bunker[b].life = dead;
						}
					}
				}
				
			}
		}
		
		// hit detect for enemy missile hitting player ship
		if (SpaceShip.life == alive)
		{
			if ((SpaceShip.x <= missile_x0) && (SpaceShip.x+SpaceShip.w >= missile_x1))
			{
				if (SpaceShip.y+SpaceShip.h <= missile_y1)
				{
					// if lives = 1 and just got hit, set game status sephmore to gameover
					if (lives == 1)
						game_status = GameOver;
					// if lives > 1, dec lives
					if (lives > 1)
						lives--;
				}
			}
			
			// hit detect for enemy laser hitting player ship
			if ((SpaceShip.x <= laser_x0) && (SpaceShip.x+SpaceShip.w >= laser_x1))
			{
				if (SpaceShip.y+SpaceShip.h <= laser_y1)
				{
					if (lives == 1)
						game_status = GameOver;
					
					if (lives > 1)
						lives--;
						
				}
			}
			
		}
		
		// floor check for enemy laser and missile
		if (missile_y1 >= SPACESHIP_START_Y+SpaceShip.h)
		{
			Enemy_Missile.life = dead;
		}
		
		if (laser_y1 >= SPACESHIP_START_Y+SpaceShip.h)
		{
			Enemy_Laser.life = dead;
		}
		
	}
	
	
			
}

