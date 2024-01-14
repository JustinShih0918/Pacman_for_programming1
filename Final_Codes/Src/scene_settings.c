// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "utility.h"
#include "scene_game.h"
#include "game.h"
#include <allegro5/allegro_primitives.h>
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */
static ALLEGRO_BITMAP* dropbox_icon;
static Checkbox checkbox_1;
static Checkbox checkbox_dropbox_icon;
static Checkbox checkbox_key;
static Checkbox window;
static Checkbox button_up;
static Checkbox button_down;
static Checkbox button_right;
static Checkbox button_left;
static ALLEGRO_SAMPLE_ID settingBGM;
static ALLEGRO_TIMER* warning;
static int up = ALLEGRO_KEY_W;
static int down = ALLEGRO_KEY_S;
static int left = ALLEGRO_KEY_A;
static int right = ALLEGRO_KEY_D;
static char text_up = 'W';
static char text_down = 'S';
static char text_left = 'A';
static char text_right = 'D';
static int warning_type = 0;
// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

bool check_key(int key_code){
	if(key_code == ALLEGRO_KEY_K) return false;
	if(key_code == 3) return false;
	if(key_code<1) return false;
	if(key_code>36) return false;

	return true;
}

bool getMode(){
	return checkbox_1.clicked;
}

int get_control_key(char d){
	switch (d)
	{
	case 'U':
		return up;
		break;
	case 'D':
		return down;
		break;
	case 'L':
		return left;
		break;
	case 'R':
		return right;
		break;
	}
}

char set_text_key(int key_code){
	char text;
	if(key_code>=1&&key_code<=26){
		for(int i = 1;i<=26;i++){
			if(key_code == i){
				text = (i+64);
				break;
			}
		}
	}else if(key_code>=27&&key_code<=36){
		for(int i = 27;i<=36;i++){
			if(key_code == i){
				text = (i+21);
				break;
			}
		}
	}
	return text;
}

Checkbox createCheckbox(int x,int y,int w,int h,bool hover,bool click){
	Checkbox checkbox;
	checkbox.rec.x = x;
	checkbox.rec.y = y;
	checkbox.rec.w = w;
	checkbox.rec.h = h;
	checkbox.x2 = x+w;
	checkbox.y2 = y+h;
	checkbox.hovered = hover;
	checkbox.clicked = click;
	return checkbox;
}

int getDropbox(){
	if(dropbox_4.clicked) return 4;
	else if(dropbox_1.clicked) return 1;
	else if(dropbox_2.clicked) return 2;
	else if(dropbox_3.clicked) return 3;
	else return 1;
}

static void init(){
	checkbox_1 = createCheckbox(100,150,80,80,false,checkbox_1.clicked);

	dropbox_icon = load_bitmap("Assets/music_icon.jpg");
	checkbox_dropbox_icon = createCheckbox(100,250,80,80,false,false);
	dropbox_1 = createCheckbox(200,350,300,50,dropbox_1.hovered,dropbox_1.clicked);
	dropbox_2 = createCheckbox(200,410,300,50,dropbox_2.hovered,dropbox_2.clicked);
	dropbox_3 = createCheckbox(200,470,300,50,dropbox_3.hovered,dropbox_3.clicked);
	dropbox_4 = createCheckbox(200,530,300,50,dropbox_4.hovered,dropbox_4.clicked);

	checkbox_key = createCheckbox(100,80,600,50,false,false);
	window = createCheckbox(100,120,600,600,false,false);
	button_up = createCheckbox(350,300,100,100,false,false);
	button_down = createCheckbox(350,410,100,100,false,false);
	button_left = createCheckbox(240,410,100,100,false,false);
	button_right = createCheckbox(460,410,100,100,false,false);

	warning = al_create_timer(1);
	stop_bgm(settingBGM);
}

static void draw_warning(int warning_type){
	switch (warning_type)
	{
	case 1:
		al_draw_text(font_pirulen_24,al_map_rgb(255,0,0),10,10,0,"The key You Press is Not Allow, Please Choose Another One");
		break;
	case 2:
		al_draw_text(font_pirulen_24,al_map_rgb(255,0,0),10,10,0,"Please turn off the Multiplayer Competititon Mode and try again");
		break;
	default:
		break;
	}
}

static void draw_dropboxs(){
	if(dropbox_2.hovered||dropbox_3.hovered||dropbox_4.hovered)
		al_draw_filled_rounded_rectangle(dropbox_1.rec.x,dropbox_1.rec.y,dropbox_1.x2,dropbox_1.y2,5,5,al_map_rgb(255, 255, 255));
	else if(dropbox_1.hovered)
		al_draw_filled_rounded_rectangle(dropbox_1.rec.x,dropbox_1.rec.y,dropbox_1.x2,dropbox_1.y2,5,5,al_map_rgb(135, 206, 235));
	else if(dropbox_2.clicked||dropbox_3.clicked||dropbox_4.clicked)
		al_draw_filled_rounded_rectangle(dropbox_1.rec.x,dropbox_1.rec.y,dropbox_1.x2,dropbox_1.y2,5,5,al_map_rgb(255, 255, 255));
	else
		al_draw_filled_rounded_rectangle(dropbox_1.rec.x,dropbox_1.rec.y,dropbox_1.x2,dropbox_1.y2,5,5,al_map_rgb(135, 206, 235));
	
	if(dropbox_1.hovered||dropbox_3.hovered||dropbox_4.hovered)
		al_draw_filled_rounded_rectangle(dropbox_2.rec.x,dropbox_2.rec.y,dropbox_2.x2,dropbox_2.y2,5,5,al_map_rgb(255, 255, 255));
	else if(dropbox_2.clicked)
		al_draw_filled_rounded_rectangle(dropbox_2.rec.x,dropbox_2.rec.y,dropbox_2.x2,dropbox_2.y2,5,5,al_map_rgb(135, 206, 235));
	else if(dropbox_2.hovered)
		al_draw_filled_rounded_rectangle(dropbox_2.rec.x,dropbox_2.rec.y,dropbox_2.x2,dropbox_2.y2,5,5,al_map_rgb(135, 206, 235));
	else
		al_draw_filled_rounded_rectangle(dropbox_2.rec.x,dropbox_2.rec.y,dropbox_2.x2,dropbox_2.y2,5,5,al_map_rgb(255, 255, 255));

	if(dropbox_2.hovered||dropbox_1.hovered||dropbox_4.hovered)
		al_draw_filled_rounded_rectangle(dropbox_3.rec.x,dropbox_3.rec.y,dropbox_3.x2,dropbox_3.y2,5,5,al_map_rgb(255, 255, 255));
	else if(dropbox_3.clicked)
		al_draw_filled_rounded_rectangle(dropbox_3.rec.x,dropbox_3.rec.y,dropbox_3.x2,dropbox_3.y2,5,5,al_map_rgb(135, 206, 235));
	else if(dropbox_3.hovered)
		al_draw_filled_rounded_rectangle(dropbox_3.rec.x,dropbox_3.rec.y,dropbox_3.x2,dropbox_3.y2,5,5,al_map_rgb(135, 206, 235));
	else
		al_draw_filled_rounded_rectangle(dropbox_3.rec.x,dropbox_3.rec.y,dropbox_3.x2,dropbox_3.y2,5,5,al_map_rgb(255, 255, 255));
	
	if(dropbox_2.hovered||dropbox_1.hovered||dropbox_3.hovered)
		al_draw_filled_rounded_rectangle(dropbox_4.rec.x,dropbox_4.rec.y,dropbox_4.x2,dropbox_4.y2,5,5,al_map_rgb(255, 255, 255));
	else if(dropbox_4.clicked)
		al_draw_filled_rounded_rectangle(dropbox_4.rec.x,dropbox_4.rec.y,dropbox_4.x2,dropbox_4.y2,5,5,al_map_rgb(135, 206, 235));
	else if(dropbox_4.hovered)
		al_draw_filled_rounded_rectangle(dropbox_4.rec.x,dropbox_4.rec.y,dropbox_4.x2,dropbox_4.y2,5,5,al_map_rgb(135, 206, 235));
	else
		al_draw_filled_rounded_rectangle(dropbox_4.rec.x,dropbox_4.rec.y,dropbox_4.x2,dropbox_4.y2,5,5,al_map_rgb(255, 255, 255));

	al_draw_text(font_pirulen_32,al_map_rgb(0,0,0),(dropbox_1.rec.x+dropbox_1.x2)/2,(dropbox_1.rec.y+dropbox_1.y2)/2-25,ALLEGRO_ALIGN_CENTRE,"Pacman");
	al_draw_text(font_pirulen_24,al_map_rgb(0,0,0),(dropbox_2.rec.x+dropbox_2.x2)/2,(dropbox_2.rec.y+dropbox_2.y2)/2-19,ALLEGRO_ALIGN_CENTRE,"Mission Impossible");
	al_draw_text(font_pirulen_32,al_map_rgb(0,0,0),(dropbox_3.rec.x+dropbox_3.x2)/2,(dropbox_3.rec.y+dropbox_3.y2)/2-25,ALLEGRO_ALIGN_CENTRE,"Interstellar");
	al_draw_text(font_pirulen_32,al_map_rgb(0,0,0),(dropbox_4.rec.x+dropbox_4.x2)/2,(dropbox_4.rec.y+dropbox_4.y2)/2-25,ALLEGRO_ALIGN_CENTRE,"Mute Music");
}

static void draw_dropbox_icon(){
	if(dropbox_2.clicked)
		al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,265,0,"Mission Impossible");
	else if(dropbox_3.clicked)
		al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,265,0,"Interstellar");
	else if(dropbox_4.clicked)
		al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,265,0,"Mute Music");
	else
		al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,265,0,"Pacman");
	
	if(checkbox_dropbox_icon.clicked){
		al_draw_scaled_bitmap(dropbox_icon,222,14,80,80,checkbox_dropbox_icon.rec.x,checkbox_dropbox_icon.rec.y,80,80,0);
		draw_dropboxs();
		al_draw_scaled_bitmap(dropbox_icon,222,14,80,80,checkbox_dropbox_icon.rec.x,checkbox_dropbox_icon.rec.y,80,80,0);
	}
	else if(checkbox_dropbox_icon.hovered){
		al_draw_scaled_bitmap(dropbox_icon,222,14,80,80,checkbox_dropbox_icon.rec.x,checkbox_dropbox_icon.rec.y,80,80,0);
	}
	else if(!checkbox_dropbox_icon.clicked&&!checkbox_dropbox_icon.hovered){
		al_draw_scaled_bitmap(dropbox_icon,143,14,80,80,checkbox_dropbox_icon.rec.x,checkbox_dropbox_icon.rec.y,80,80,0);
	}
}


static void draw_checkbox_1(){
	al_draw_text(font_pirulen_32,al_map_rgb(255,255,255),200,165,0,"Multiplayer Competition Mode");

	if(checkbox_1.clicked){
		al_draw_filled_rounded_rectangle(checkbox_1.rec.x,checkbox_1.rec.y,checkbox_1.x2,checkbox_1.y2,5,5,al_map_rgb(255,255,255));
		al_draw_circle(140,190,30,al_map_rgb(0,0,0),5);
	}
	else if(checkbox_1.hovered){
		al_draw_filled_rounded_rectangle(checkbox_1.rec.x,checkbox_1.rec.y,checkbox_1.x2,checkbox_1.y2,5,5,al_map_rgb(255,255,255));
		al_draw_circle(140,190,30,al_map_rgb(0,0,0),5);
	}
	else if(!checkbox_1.clicked&&!checkbox_1.hovered){
		al_draw_filled_rounded_rectangle(checkbox_1.rec.x,checkbox_1.rec.y,checkbox_1.x2,checkbox_1.y2,5,5,al_map_rgb(255,255,255));
	}
}

static void draw_window(){
	al_draw_filled_rounded_rectangle(window.rec.x,window.rec.y,window.x2,window.y2,5,5,al_map_rgb(128,128,128));

	if(button_up.clicked){
		al_draw_filled_rounded_rectangle(button_up.rec.x,button_up.rec.y,button_up.x2,button_up.y2,5,5,al_map_rgb(255,255,0));
		al_draw_textf(font_pirulen_32,al_map_rgb(255,0,0),button_up.rec.x+47,button_up.rec.y+30,1,"%c",text_up);
	}
	else{
		al_draw_filled_rounded_rectangle(button_up.rec.x,button_up.rec.y,button_up.x2,button_up.y2,5,5,al_map_rgb(255,255,255));
		al_draw_textf(font_pirulen_32,al_map_rgb(0,0,0),button_up.rec.x+47,button_up.rec.y+30,1,"%c",text_up);
	}

	if(button_down.clicked){
		al_draw_filled_rounded_rectangle(button_down.rec.x,button_down.rec.y,button_down.x2,button_down.y2,5,5,al_map_rgb(255,255,0));
		al_draw_textf(font_pirulen_32,al_map_rgb(255,0,0),button_down.rec.x+47,button_down.rec.y+30,1,"%c",text_down);
	}
	else{
		al_draw_filled_rounded_rectangle(button_down.rec.x,button_down.rec.y,button_down.x2,button_down.y2,5,5,al_map_rgb(255,255,255));
		al_draw_textf(font_pirulen_32,al_map_rgb(0,0,0),button_down.rec.x+47,button_down.rec.y+30,1,"%c",text_down);
	}

	if(button_left.clicked){
		al_draw_filled_rounded_rectangle(button_left.rec.x,button_left.rec.y,button_left.x2,button_left.y2,5,5,al_map_rgb(255,255,0));
		al_draw_textf(font_pirulen_32,al_map_rgb(255,0,0),button_left.rec.x+47,button_left.rec.y+30,1,"%c",text_left);
	}
	else{
		al_draw_filled_rounded_rectangle(button_left.rec.x,button_left.rec.y,button_left.x2,button_left.y2,5,5,al_map_rgb(255,255,255));
		al_draw_textf(font_pirulen_32,al_map_rgb(0,0,0),button_left.rec.x+47,button_left.rec.y+30,1,"%c",text_left);
	}

	if(button_right.clicked){
		al_draw_filled_rounded_rectangle(button_right.rec.x,button_right.rec.y,button_right.x2,button_right.y2,5,5,al_map_rgb(255,255,0));
		al_draw_textf(font_pirulen_32,al_map_rgb(255,0,0),button_right.rec.x+47,button_right.rec.y+30,1,"%c",text_right);
	}
	else{
		al_draw_filled_rounded_rectangle(button_right.rec.x,button_right.rec.y,button_right.x2,button_right.y2,5,5,al_map_rgb(255,255,255));
		al_draw_textf(font_pirulen_32,al_map_rgb(0,0,0),button_right.rec.x+47,button_right.rec.y+30,1,"%c",text_right);
	}
}

static void draw_checkbox_key(){
	if(checkbox_key.clicked){
		al_draw_filled_rounded_rectangle(checkbox_key.rec.x,checkbox_key.rec.y,checkbox_key.x2,checkbox_key.y2,5,5,al_map_rgb(128,128,128));
		draw_window();
	}else if(checkbox_key.hovered){
		al_draw_filled_rounded_rectangle(checkbox_key.rec.x,checkbox_key.rec.y,checkbox_key.x2,checkbox_key.y2,5,5,al_map_rgb(128,128,128));
	}
	else if(!checkbox_key.clicked&&!checkbox_key.hovered){
		al_draw_filled_rounded_rectangle(checkbox_key.rec.x,checkbox_key.rec.y,checkbox_key.x2,checkbox_key.y2,5,5,al_map_rgb(255,255,255));
	}
	al_draw_text(font_pirulen_32,al_map_rgb(0,0,0),400,80,ALLEGRO_ALIGN_CENTRE,"Change Control Key");
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
	draw_dropbox_icon();
	draw_checkbox_key();
	if(al_get_timer_started(warning)){
		draw_warning(warning_type);
	}

	if(al_get_timer_count(warning) == 1){
		al_stop_timer(warning);
		al_set_timer_count(warning,0);
	}
}

static bool checkboxHover(RecArea rec, int mouse_x, int mouse_y) {
	// TODO-HACKATHON 3-6: Check if mouse is hovering on the button
	//	Uncomment and fill the code below
	
	return pnt_in_rect(mouse_x,mouse_y,rec);
	
	return false;
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f){
	checkbox_key.hovered = checkboxHover(checkbox_key.rec,mouse_x,mouse_y);

	if(!checkbox_key.clicked){
		checkbox_1.hovered = checkboxHover(checkbox_1.rec,mouse_x,mouse_y);
		checkbox_dropbox_icon.hovered = checkboxHover(checkbox_dropbox_icon.rec,mouse_x,mouse_y);
	}
	if(checkbox_dropbox_icon.clicked){
		dropbox_1.hovered = checkboxHover(dropbox_1.rec,mouse_x,mouse_y);
		dropbox_2.hovered = checkboxHover(dropbox_2.rec,mouse_x,mouse_y);
		dropbox_3.hovered = checkboxHover(dropbox_3.rec,mouse_x,mouse_y);
		dropbox_4.hovered = checkboxHover(dropbox_4.rec,mouse_x,mouse_y);
	}
	if(checkbox_key.clicked){
		button_up.hovered = checkboxHover(button_up.rec,mouse_x,mouse_y);
		button_down.hovered = checkboxHover(button_down.rec,mouse_x,mouse_y);
		button_left.hovered = checkboxHover(button_left.rec,mouse_x,mouse_y);
		button_right.hovered = checkboxHover(button_right.rec,mouse_x,mouse_y);
	}
}

static void on_mouse_down() {
	if(checkbox_1.hovered)	checkbox_1.clicked = !checkbox_1.clicked;
	if(checkbox_dropbox_icon.hovered) checkbox_dropbox_icon.clicked = !checkbox_dropbox_icon.clicked;
	if(checkbox_key.hovered&&!checkbox_1.clicked){
		checkbox_key.clicked = !checkbox_key.clicked;
		button_up.clicked = false;
		button_down.clicked = false;
		button_left.clicked = false;
		button_right.clicked = false;
	}
	else if(checkbox_key.hovered && checkbox_1.clicked){
		al_start_timer(warning);
		warning_type = 2;
		game_log("Please turn off the Multiplayer Competititon Mode and try again");
	}


	if(dropbox_1.hovered){
		dropbox_1.clicked = true; 
		dropbox_2.clicked = false;
		dropbox_3.clicked = false;
		dropbox_4.clicked = false;
	}
	else if(dropbox_2.hovered){
		dropbox_1.clicked = false;
		dropbox_2.clicked = true;
		dropbox_3.clicked = false;
		dropbox_4.clicked = false;
	}
	else if(dropbox_3.hovered){
		dropbox_1.clicked = false;
		dropbox_2.clicked = false;
		dropbox_3.clicked = true;
		dropbox_4.clicked = false;
	}
	else if(dropbox_4.hovered){
		dropbox_1.clicked = false;
		dropbox_2.clicked = false;
		dropbox_3.clicked = false;
		dropbox_4.clicked = true;
	}

	if(button_up.hovered){
		button_up.clicked = !button_up.clicked;
		button_down.clicked = false;
		button_left.clicked = false;
		button_right.clicked = false;
	}
	else if(button_down.hovered){
		button_up.clicked = false;
		button_down.clicked = !button_down.clicked;
		button_left.clicked = false;
		button_right.clicked = false;
	}
	else if(button_left.hovered){
		button_up.clicked = false;
		button_down.clicked = false;
		button_left.clicked = !button_left.clicked;
		button_right.clicked = false;
	}
	else if(button_right.hovered){
		button_up.clicked = false;
		button_down.clicked = false;
		button_left.clicked = false;
		button_right.clicked = !button_right.clicked;
	}
}

static void on_key_down(int keycode) {
	if(button_up.clicked){
		if(check_key(keycode)){
			up = keycode;
			text_up = set_text_key(keycode);
			set_control_key(up,down,left,right);
		}
		else{
			al_start_timer(warning);
			warning_type = 1;
			game_log("The key You Press is Not Allow, Please Choose Another One");
		}
	}
	else if(button_down.clicked){
		if(check_key(keycode)){
			down = keycode;
			text_down = set_text_key(keycode);
			set_control_key(up,down,left,right);
		}
		else{
			al_start_timer(warning);
			warning_type = 1;
			game_log("The key You Press is Not Allow, Please Choose Another One");
		}
	}
	else if(button_left.clicked){
		if(check_key(keycode)){
			left = keycode;
			text_left = set_text_key(keycode);
			set_control_key(up,down,left,right);
		}
		else{
			al_start_timer(warning);
			warning_type = 1;
			game_log("The key You Press is Not Allow, Please Choose Another One");
		}
	}
	else if(button_right.clicked){
		if(check_key(keycode)){
			right = keycode;
			text_right = set_text_key(keycode);
			set_control_key(up,down,left,right);
		}
		else{
			al_start_timer(warning);
			warning_type = 1;
			game_log("The key You Press is Not Allow, Please Choose Another One");
		}
	}
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			break;
		default:
			break;
	}
}

static void destroy_setting(){
	al_destroy_bitmap(dropbox_icon);
	al_destroy_timer(warning);
	stop_bgm(settingBGM);
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
	scene.destroy = &destroy_setting;
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