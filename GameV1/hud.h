#ifndef HUD_H
#define HUD_H

void Hud_Menu(int score, int highscore);
void Hud_Ingame(int score, int highscore, int lives, int level);

void Hud_Clear_Upper(void);
void Hud_Clear_Lower(void);
void Hud_Clear_All(void);

#endif
