#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void Menu(int style);
void SubMenu(int style);

void Start_Menu(void);
void GameOver_Menu(void);
//void ScoreTable_SubMenu(void);

void Menu_Clear(void);
void SubMenu_Clear(void);
void Player_Ship_Clear(void);

void Text_Delay_10ms(uint32_t n);

#endif
