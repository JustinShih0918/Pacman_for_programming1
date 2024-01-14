// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

static int controlUp;
static int controlDown;
static int controlLeft;
static int controlRight;

static int ghost_controlUp;
static int ghost_controlDown;
static int ghost_controlLeft;
static int ghost_controlRight;
static int ghost_control_index = 0;
Scene scene_main_create(void);
/*
int64_t get_power_up_timer_tick();
int64_t get_power_up_duration();
*/

int64_t get_PowerUp_Time();
bool get_ghost_stop();
bool get_pacman_smash();
void set_control_key(int up,int down,int left,int right);
int get_ghost_control_index();
#endif
