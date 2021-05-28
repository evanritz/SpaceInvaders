// hud.c

// Draws score, highscore, lives, and level indicators for user

#include <string.h>
#include <stdio.h>

#include "hud.h"
#include "ST7735.h"
#include "bitmaps.h"
#include "display_funcs.h"
#include "display_consts.h"


void Hud_Menu(int score, int highscore)
{
	char score_txt[] 	= "Score<1> Hi-Score<2>";

	int x = MID_X - (strlen(score_txt)*W_S)/2;
	int y = UPPER_HUD_TOP_LEFT_Y;
	
	Draw_White_Text(x, y, score_txt);

	char score_val[4];
	sprintf(score_val, "%d", score);

	y = UPPER_HUD_TOP_LEFT_Y + W_H;
	x = W_S;

	Draw_White_Text(x, y, score_val);

	char highscore_val[4];
	sprintf(highscore_val, "%d", highscore);
	
	y = UPPER_HUD_TOP_LEFT_Y + W_H;
	x = W_S*9;
	
	Draw_White_Text(x, y, highscore_val);
}
void Hud_Ingame(int score, int highscore, int lives, int level)
{

	Hud_Menu(score, highscore);
	
	char live_val[1];
	sprintf(live_val, "%d", lives);
	
	int x = LOWER_HUD_TOP_LEFT_X;
	int y = LOWER_HUD_TOP_LEFT_Y + 13;
	
	Draw_White_Text(x, y, live_val);
	
	x = LOWER_HUD_TOP_LEFT_X + W_S;
	y = LOWER_HUD_TOP_LEFT_Y + 19;
	
	int black = 3-lives;
	for (int i = 0; i < lives; i++)
	{	
		ST7735_DrawBitmap(x, y, PlayerShip0, 18, 8);
		x += 18;
	}
	for (int i = 0; i < black; i++)
	{	
		ST7735_DrawBitmap(x, y, PlayerShip1, 18, 8);
		x += 18;
	}
	
	char level_val[4];
	sprintf(level_val, "%d", level);
	
	x = LOWER_HUD_BOTTOM_RIGHT_X-W_S*strlen(level_val);
	y = LOWER_HUD_TOP_LEFT_Y + 13;
	Draw_White_Text(x, y, level_val);
	
}


void Hud_Clear_Upper(void)
{
	Clear(UH);
}

void Hud_Clear_Lower(void)
{
	Clear(LH);
}

void Hud_Clear_All(void)
{
	Clear(UH);
	Clear(LH);
}

