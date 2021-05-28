// Menu.c

// Draws text for start and gameover menus


#include <stdio.h>
#include <string.h>

#include "display_consts.h"
#include "Menu.h"
#include "ST7735.h"
#include "joystick.h"
#include "display_funcs.h"
#include "game_vars.h"
#include "game_consts.h"


void Menu(int style)
{
	switch(style)
	{
		case Running:
			Start_Menu();
			break;
		case GameOver:
			GameOver_Menu();
			break;
	}
}

void Start_Menu(void)
{

	char start_txt1[] = "PLAY  ";
	char start_txt2[] = "SPACE INVADERS";
	char start_txt3[] = "(Hold to start)";
	
	int x = MID_X - (strlen(start_txt1)*W_S)/2;
	int y = MENU_TOP_LEFT_Y + W_H;
	
	Draw_White_Text_Delayed_10ms(x, y, start_txt1, 35);

	x = MID_X - (strlen(start_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_White_Text_Delayed_10ms(x, y, start_txt2, 35);
	
	x = MID_X - (strlen(start_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	Draw_White_Text(x, y, start_txt3);
}

void GameOver_Menu(void)
{
	
	Menu_Clear();
	SubMenu_Clear();
	
	char gameover_txt1[] = "GAME OVER";
	char gameover_txt2[] = "(Hold to start)";
	
	int x = MID_X - (strlen(gameover_txt1)*W_S)/2;
	int y = MENU_TOP_LEFT_Y + 3*W_H;
	
	Draw_Text(x, y, gameover_txt1, WHITE, 1);
	
	x = MID_X - (strlen(gameover_txt2)*W_S)/2;
	y = MENU_TOP_LEFT_Y + 6*W_H;
	
	Draw_Text(x, y, gameover_txt2, WHITE, 1);
	
}

void SubMenu(int style)
{
	// todo
}

void Player_Ship_Clear(void)
{
	ST7735_FillRect(MIN_X, SpaceShip.y-SpaceShip.h+1, MAX_X, SpaceShip.h, BLACK);
}

void Text_Delay_10ms(uint32_t n)
{
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

void Menu_Clear(void)
{
	Clear(M);
}

void SubMenu_Clear(void)
{
	Clear(SM);
}
