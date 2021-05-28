#ifndef DISPLAY_FUNCS_H
#define DISPLAY_FUNCS_H

#include <stdint.h>

#define BLACK 0x0000
#define WHITE 0xFFFF

// Areas on Screen
#define UH 1 
#define LH 2
#define M  3
#define SM 4

void Clear(int area);

//void Draw_Bitmap(int x, int y, const unsigned short *bitmap

void Draw_Text(int x, int y, char *txt, uint16_t color, int size);
void Draw_Text_Delayed_10ms(int x, int y, char *txt, uint16_t color, int size, int delay);

void Draw_White_Text(int x, int y, char *txt);
void Draw_White_Text_Delayed_10ms(int x, int y, char *txt, int delay);

void Delay_10ms(int delay);

#endif
