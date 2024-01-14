#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_settings.h"

// TODO-HACKATHON 2-0: Create one ghost
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
// Finish
// TODO-GC-ghost: create a least FOUR ghost!
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
int bean_ate_amount = 0;
bool game_over = false;

/* Internal variables*/
static int preCode;
static ALLEGRO_SAMPLE_ID Background_Music;
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool pacman_smash = false;
bool ghost_stop = false;
bool debug_mode = false;
bool cheat_mode = false;
/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(int btn, int x, int y, int dz);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	pacman_smash = false;
	ghost_stop = false;
	cheat_mode = false;
	game_over = false;
	game_main_Score = 0;
	bean_ate_amount = 0;
	// create map
	//basic_map = create_map(NULL);
	// Finish
	// TODO-GC-read_txt: Create map from .txt file so that you can design your own map!!
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// TODO-HACKATHON 2-1: Allocate dynamic memory for ghosts array.
	ghosts = (Ghost*)malloc(sizeof(Ghost) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// TODO-HACKATHON 2-2: create a ghost.
		// Try to look the definition of ghost_create in ghost.h and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
			
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick per second
	if (!power_up_timer)
		game_abort("Error on create timer\n");

	stop_bgm(Background_Music);
	switch (getDropbox())
	{
	case 1:
		Background_Music = play_bgm(PACMAN_themeMusic, music_volume);
		break;
	case 2:
		Background_Music = play_bgm(MissionImpossible_themeMusic,music_volume);
		break;
	case 3:
		Background_Music = play_bgm(Intersteller_themeMusic, music_volume);
		break;
	case 4:
		break;
	}

	if(getMode()){
		controlUp = ALLEGRO_KEY_UP;
		controlDown = ALLEGRO_KEY_DOWN;
		controlLeft = ALLEGRO_KEY_LEFT;
		controlRight = ALLEGRO_KEY_RIGHT;

		ghost_controlUp = ALLEGRO_KEY_W;
		ghost_controlLeft = ALLEGRO_KEY_A;
		ghost_controlDown = ALLEGRO_KEY_S;
		ghost_controlRight = ALLEGRO_KEY_D;
	}
	else{
		controlUp = get_control_key('U');
		controlDown = get_control_key('D');
		controlRight = get_control_key('R');
		controlLeft = get_control_key('L');
	}

	return;
}

static void change_control_ghost(){
	ghost_control_index++;
	if(ghost_control_index>=GHOST_NUM)
		ghost_control_index = 0;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// TODO-HACKATHON 1-3: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman,'.');
		basic_map->map[Grid_y][Grid_x] = ' ';
		game_main_Score++;
		bean_ate_amount++;
		break;
	case 'P':
		// Finish
		// TODO-GC-PB: ease power bean
		pacman_eatItem(pman,'P');
		basic_map->map[Grid_y][Grid_x] = ' ';
		al_set_timer_count(power_up_timer,0);
		al_start_timer(power_up_timer);
		// stop and reset power_up_timer value
		// start the timer
		break;
	default:
		break;
	}

	// TODO-HACKATHON 1-4: erase the item you eat from map
	// Be careful, don't erase the wall block.
	
}
static void status_update(void) {
	// TODO-PB: check powerUp duration
	if(bean_ate_amount == basic_map->beansNum){
		game_log("Game_complete!");
		stop_bgm(Background_Music);
		pacman_victory();
		game_over = true;
	}
	if (pman->powerUp)
	{
		// Check the value of power_up_timer
		// If runs out of time reset all relevant variables and ghost's status
		// hint: ghost_toggle_FLEE
		if(get_PowerUp_Time()==power_up_duration){
			for(int i = 0;i<GHOST_NUM;i++){
				ghost_toggle_FLEE(ghosts[i],false);
			}
			pman->powerUp = false;
		}
		else{
			for(int i = 0;i<GHOST_NUM;i++){
				ghost_toggle_FLEE(ghosts[i],true);
			}
		}
		
	}
	


	for (int i = 0; i < GHOST_NUM; i++) {
		const RecArea ghostRec = getDrawArea(ghosts[i],GAME_TICK_CD);
		const RecArea pmanRec = getDrawArea(pman,GAME_TICK_CD);
		if (ghosts[i]->status == GO_IN){
			continue;
		}
		else if (ghosts[i]->status == FREEDOM)
		{	
			// Finish
			// TODO-GC-game_over: use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect if pacman and ghosts collide with each other.
			// And perform corresponding operations.
			// [NOTE] You should have some if-else branch here if you want to implement power bean mode.
			// Uncomment Following Code
			
			if(!cheat_mode && RecAreaOverlap(&pmanRec, &ghostRec))
			{
				game_log("collide with ghost\n");
				stop_bgm(Background_Music);
				al_rest(1.0);
				pacman_die();
				game_over = true;
				break; // animation shouldn't be trigger twice.
			}
			
		}
		else if (ghosts[i]->status == FLEE)
		{
			// TODO-GC-PB: if ghost is collided by pacman, it should go back to the cage immediately and come out after a period.
			// Finish
			if(!cheat_mode && RecAreaOverlap(&pmanRec, &ghostRec))
			{
				pacman_eatGhost();
				ghost_collided(ghosts[i]);
				game_main_Score+=20;
			}
			else if(cheat_mode){
				pacman_eatGhost();
				ghost_collided(ghosts[i]);
			}
			
		}
	}
}

static void update(void) {

	if (game_over) {
		// Finish
		// TODO-GC-game_over: start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
		// hint: refer al_get_timer_started(...), al_get_timer_count(...), al_stop_timer(...), al_rest(...)
		/*
			// start the timer if it hasn't been started.
			// check timer counter.
			// stop the timer if counter reach desired time.
			game_change_scene(...);
		*/
		if(!al_get_timer_started(pman->death_anim_counter)){
			al_start_timer(pman->death_anim_counter);
			al_set_timer_count(pman->death_anim_counter,0);
		}
		if(get_pacmanDie_value()>=300){
			change_pacmanDie_value(0,0);
			al_set_timer_count(pman->death_anim_counter,0);
			al_stop_timer(pman->death_anim_counter);
			game_change_scene(scene_menu_create());
		}
		
		return;
	}

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	// TODO-GC-scoring: Draw scoreboard, something your may need is sprinf();
	// Finish
	
	al_draw_textf(font_pirulen_32,al_map_rgb(255,255,0),10,10,ALLEGRO_ALIGN_LEFT,"%s: %d","Your score",game_main_Score);
	al_draw_textf(font_pirulen_32,al_map_rgb(255,255,0),300,10,ALLEGRO_ALIGN_LEFT,"%s: %d","bean_eaten",bean_ate_amount);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	// Memory
	// TODO-GC-memory: free map array, Pacman and ghosts
	pacman_destroy(pman);
	for(int i = 0;i<GHOST_NUM;i++){
		ghost_destroy(ghosts[i]);
	}
	delete_map(basic_map);
	stop_bgm(Background_Music);
}

static void on_key_down(int key_code) {
	if(getMode()){
		if(key_code == ghost_controlUp)
			ghost_NextMove(ghosts[ghost_control_index],UP);
		else if(key_code == ghost_controlDown)
			ghost_NextMove(ghosts[ghost_control_index],DOWN);
		else if(key_code == ghost_controlLeft)
			ghost_NextMove(ghosts[ghost_control_index],LEFT);
		else if(key_code == ghost_controlRight)
			ghost_NextMove(ghosts[ghost_control_index],RIGHT);
	}

	if(key_code == controlUp)
		pacman_NextMove(pman,UP);
	else if(key_code == controlDown)
		pacman_NextMove(pman,DOWN);
	else if(key_code == controlLeft)
		pacman_NextMove(pman,LEFT);
	else if(key_code == controlRight)
		pacman_NextMove(pman,RIGHT);
		
	switch (key_code)
	{
		// TODO-HACKATHON 1-1: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		case ALLEGRO_KEY_S:
			if(cheat_mode){
				if(preCode == ALLEGRO_KEY_COMMAND){
					ghost_stop = !ghost_stop;
					if (ghost_stop)
					printf("Ghost Stop\n");
					else 
					printf("cGhost Move\n");
				}
			}
			break;
		case ALLEGRO_KEY_C:
			if(getMode()){
				change_control_ghost();
			}
			else{
				cheat_mode = !cheat_mode;
				if (cheat_mode)
					printf("cheat mode on\n");
				else 
					printf("cheat mode off\n");
			}
			break;
		case ALLEGRO_KEY_K:
			if(cheat_mode){
				for(int i = 0;i<GHOST_NUM;i++){
				ghosts[i]->status = GO_IN;
				ghosts[i]->speed = 4;
				}
			}
			break;
		case ALLEGRO_KEY_G:
			if(controlDown == ALLEGRO_KEY_G || controlLeft == ALLEGRO_KEY_G || controlRight == ALLEGRO_KEY_G || controlUp == ALLEGRO_KEY_G)
				break;
			debug_mode = !debug_mode;
			break;
		case ALLEGRO_KEY_L:
			if(cheat_mode){
				if(preCode == ALLEGRO_KEY_COMMAND)
				pacman_smash = !pacman_smash;
				if (pacman_smash)
					printf("Pacman Can Cross The Wall\n");
				else 
					printf("Pacman Move Normally\n");
			}
			break;
	default:
		break;
	}
	preCode = key_code;
}

static void on_mouse_down(int btn, int x, int y, int dz) {

	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}

int64_t get_PowerUp_Time(){
	return al_get_timer_count(power_up_timer);
}

bool get_ghost_stop(){
	return ghost_stop;
}

bool get_pacman_smash(){
	return pacman_smash;
}

void set_control_key(int up,int down,int left,int right){
	controlUp = up;
	controlDown = down;
	controlLeft = left;
	controlRight = right;
}

int get_ghost_control_index(){
	return ghost_control_index;
}

