// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static Checkbox checkbox_1;
static Checkbox checkbox_2;
static ALLEGRO_SAMPLE_ID settingBGM;
// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

static void init(){

	checkbox_1.hovered = false;
	checkbox_1.clicked = false;
	checkbox_1.rec.x = 100;
	checkbox_1.rec.y = 100;
	checkbox_1.rec.w = 80;
	checkbox_1.rec.h = 80;

	stop_bgm(settingBGM);
}

static void draw_checkbox_1(){
	al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,115,0,"Change Music !");

	if(checkbox_1.clicked){
		al_draw_filled_rounded_rectangle(100,100,180,180,5,5,al_map_rgb(200,200,0));
		al_draw_circle(140,140,30,al_map_rgb(255,255,255),5);
	}
	else if(checkbox_1.hovered){
		al_draw_filled_rounded_rectangle(100,100,180,180,5,5,al_map_rgb(200,200,0));
		al_draw_circle(140,140,30,al_map_rgb(255,255,255),5);
	}
	else if(!checkbox_1.clicked&&!checkbox_1.hovered)
		al_draw_filled_rounded_rectangle(100,100,180,180,5,5,al_map_rgb(200,200,0));

	if(checkbox_1.hovered){
		stop_bgm(settingBGM);
		settingBGM = play_audio(HOVER_SOUND,effect_volume);
	}
}

static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to menu"
	);
	draw_checkbox_1();
}

static bool checkboxHover(RecArea rec, int mouse_x, int mouse_y) {
	// TODO-HACKATHON 3-6: Check if mouse is hovering on the button
	//	Uncomment and fill the code below
	
	return pnt_in_rect(mouse_x,mouse_y,rec);
	
	return false;
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f){
	checkbox_1.hovered = checkboxHover(checkbox_1.rec,mouse_x,mouse_y);
}

static void on_mouse_down() {
	if (checkbox_1.hovered){
		checkbox_1.clicked = !checkbox_1.clicked;
	}
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			break;
		default:
			break;
	}
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.initialize = &init;
	scene.name = "Settings";
	scene.draw = &draw;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

// TODO-Graphical_Widget:
// Just suggestions, you can create your own usage.
	// Selecting BGM:
	// 1. Declare global variables for storing the BGM. (see `shared.h`, `shared.c`)
	// 2. Load the BGM in `shared.c`.
	// 3. Create dropdown menu for selecting BGM in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and play the BGM if the corresponding option is selected.

	// Adjusting Volume:
	// 1. Declare global variables for storing the volume. (see `shared.h`, `shared.c`)
	// 2. Create a slider for adjusting volume in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. You may use checkbox to switch between mute and unmute.
	// 3. Adjust the volume and play when the button is pressed.

	// Selecting Map:
	// 1. Preload the map to `shared.c`.
	// 2. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. For player experience, you may also create another scene between menu scene and game scene for selecting map.
	// 3. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and draw the map if the corresponding option is selected.
		// 4.1. Suggestions: You may use `al_draw_bitmap` to draw the map for previewing. 
							// But the map is too large to be drawn in original size. 
							// You might want to modify the function to allow scaling.