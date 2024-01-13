#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"

typedef struct Checkbox{
	RecArea rec;
	int x2;
	int y2;
	bool clicked;
	bool hovered;
}Checkbox;

static Checkbox dropbox_1;
static Checkbox dropbox_2;
static Checkbox dropbox_3;

Checkbox createCheckbox(int x,int y,int w,int h,bool a,bool b);
Scene scene_settings_create(void);

#endif
