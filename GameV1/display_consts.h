// display_consts.h

// holds display coords for areas on screen
// Upper HUD
// Menu
// SubMenu
// Lower HUD

#ifndef DISPLAY_CONSTS_H
#define DISPLAY_CONSTS_H

// X Spacers
#define D_S 2	// Display Spacer, Pixels from edge of display
#define W_S 6	// Word Spacer, Pixels between chars
#define M_S 5 // Menu Spacer, Pixels between hud and menu

// Y Spacers
#define W_H 10 // Word Spacer/Heighter? 

#define SP_B_X 2 // Sprite black pixels on each x side of a bitmap
#define SP_B_Y 1 // Sprite black pixels on each y side of a bitmap

// Amount of Y axis space
#define UPPER_HUD_SIZE 	20
#define LOWER_HUD_SIZE 	20
#define MENU_SIZE 		 	55
#define SUBMENU_SIZE 		55

#define MIN_Y 0
#define MIN_X 0

#define MAX_Y 159
#define MAX_X 127

#define MID_X (MAX_X/2)
#define MID_Y (MAX_Y/2)

// Upper hud
#define UPPER_HUD_TOP_LEFT_X (MIN_X + D_S)
#define UPPER_HUD_TOP_LEFT_Y (MIN_Y + D_S)

#define UPPER_HUD_BOTTOM_RIGHT_X (MAX_X - D_S)
#define UPPER_HUD_BOTTOM_RIGHT_Y (D_S + UPPER_HUD_SIZE)

// Lower hud
#define LOWER_HUD_TOP_LEFT_X (MIN_X + D_S)
#define LOWER_HUD_TOP_LEFT_Y (MAX_Y - D_S - LOWER_HUD_SIZE)

#define LOWER_HUD_BOTTOM_RIGHT_X (MAX_X - D_S)
#define LOWER_HUD_BOTTOM_RIGHT_Y (MAX_Y - D_S)

// Menu and Submenu

#define MENU_TOP_LEFT_X (MIN_X + D_S)
#define MENU_TOP_LEFT_Y (MIN_Y + D_S + UPPER_HUD_SIZE + M_S)

#define MENU_BOTTOM_RIGHT_X (MAX_X - D_S)
#define MENU_BOTTOM_RIGHT_Y (MIN_Y + D_S + UPPER_HUD_SIZE + M_S + MENU_SIZE)

#define SUBMENU_TOP_LEFT_X (MIN_X + D_S)
#define SUBMENU_TOP_LEFT_Y (MIN_Y + D_S + UPPER_HUD_SIZE + M_S + MENU_SIZE)

#define SUBMENU_BOTTOM_RIGHT_X (MAX_X - D_S)
#define SUBMENU_BOTTOM_RIGHT_Y (MAX_Y - D_S - LOWER_HUD_SIZE - M_S)//(MIN_Y + D_S + UPPER_HUD_SIZE + M_S + MENU_SIZE + SUBMENU_SIZE)


#endif
