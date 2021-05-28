#ifndef GAME_TASK_H
#define GAME_TASK_H



void Game_Task_Init(void);
void Game_Move(void);
void Shoot_SpaceShip_Laser(void);
void Level_Size(void);
void Enemy_Tally(void);
void Shoot_Enemy_Weapons(void);
void Hit_Detection(void);
void Spawn_New_SpaceShip(void);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


#endif
