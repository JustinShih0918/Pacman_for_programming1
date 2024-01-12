#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H
#include "game.h"
#include "utility.h"
#include "shared.h"
#include "scene_menu.h"

typedef struct Checkbox{
	RecArea rec;
	bool clicked;
	bool hovered;
}Checkbox;

Scene scene_settings_create(void);

#endif
