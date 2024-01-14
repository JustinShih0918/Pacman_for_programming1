// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* font_pirulen_32 = NULL;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* PACMAN_themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EAT_GHOST_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EAT_POWERBEAN_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_VICTORY_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_QUOTE = NULL;
ALLEGRO_SAMPLE* Intersteller_themeMusic = NULL;
ALLEGRO_SAMPLE* Intersteller_QUOTE = NULL;
ALLEGRO_SAMPLE* MissionImpossible_themeMusic = NULL;
ALLEGRO_SAMPLE* MissionImpossible_QUOTE = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	font_pirulen_32 = load_font("Assets/OpenSans-Regular.ttf", 32);
	font_pirulen_24 = load_font("Assets/OpenSans-Regular.ttf", 24);

	//Pacman music
	PACMAN_themeMusic = load_audio("Assets/Music/Pacman/original_theme.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/Pacman/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/Pacman/pacman_death.ogg");
	PACMAN_EAT_GHOST_SOUND = load_audio("Assets/Music/Pacman/pacman_eatghost.ogg");
	PACMAN_EAT_POWERBEAN_SOUND = load_audio("Assets/Music/Pacman/pacman_eatfruit.ogg");
	PACMAN_VICTORY_SOUND = load_audio("Assets/Music/Pacman/pacman_victory.ogg");
	PACMAN_QUOTE = load_audio("Assets/Music/Pacman/pacman_intermission.ogg");
	//Intersteller music
	Intersteller_themeMusic = load_audio("Assets/Music/Interstellar/interstellar_theme.ogg");
	Intersteller_QUOTE = load_audio("Assets/Music/Interstellar/interstellar_quote.ogg");
	//MissionImpossible music
	MissionImpossible_themeMusic = load_audio("Assets/Music/Mission_Impossible/mission_impossible_theme.ogg");
	MissionImpossible_QUOTE = load_audio("Assets/Music/Mission_Impossible/missionImpossible_quote.ogg");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);
	al_destroy_sample(PACMAN_themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_sample(PACMAN_EAT_GHOST_SOUND);
	al_destroy_sample(PACMAN_EAT_POWERBEAN_SOUND);
	al_destroy_sample(PACMAN_VICTORY_SOUND);
	al_destroy_sample(PACMAN_QUOTE);
	al_destroy_sample(Intersteller_themeMusic);
	al_destroy_sample(Intersteller_QUOTE);
	al_destroy_sample(MissionImpossible_themeMusic);
	al_destroy_sample(MissionImpossible_QUOTE);
}