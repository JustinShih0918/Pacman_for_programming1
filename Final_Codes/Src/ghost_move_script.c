#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
#include "scene_game.h"
#include "scene_settings.h"
/* Shared variables */
#define GO_OUT_TIME 256
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;
extern bool game_over;
/* Declare static function prototypes */
// static function reference: https://stackoverflow.com/questions/558122/what-is-a-static-function-in-c
static void ghost_stop_script(Ghost* ghost);
static void ghost_move(Ghost* ghost, Map* M);
static void ghost_move_script_FREEDOM_random(Ghost* ghost, Map* M);
static void ghost_move_script_FREEDOM_shortest_path(Ghost* ghost, Map* M, Pacman* pman);
static void ghost_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_move_script_GO_IN(Ghost* ghost, Map* M);
static void ghost_move_script_GO_OUT(Ghost* ghost, Map* M);
static void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman* const pacman);

static void ghost_move(Ghost* ghost, Map* M){
	if (!movetime(ghost->speed))
		return;
	if (game_over)
		return;

		switch (ghost->status)
		{
			case BLOCKED:
				if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
					ghost->status = GO_OUT;
				break;
			case FREEDOM:
				ghost->status = GO_OUT;
				break;
			case GO_OUT:
				ghost->status = GO_OUT;
				break;
			case GO_IN:
				ghost_move_script_GO_IN(ghost, M);
				if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
					ghost->status = BLOCKED;
					ghost->speed = 2; // reset the speed after back to the cage.
					ghost->go_in_time = al_get_timer_count(game_tick_timer); 
				}
				break;
			case FLEE:
				break;
			default:
				break;
		}

	int probe_x = ghost->objData.Coord.x, probe_y = ghost->objData.Coord.y;
	if (ghost_movable(ghost, M, ghost->objData.nextTryMove,false)) 
		ghost->objData.preMove = ghost->objData.nextTryMove;
	else if (!ghost_movable(ghost, M, ghost->objData.preMove,false)) 
		return;

	switch (ghost->objData.preMove)
	{
	case UP:
		ghost->objData.Coord.y -= 1;
		ghost->objData.preMove = UP;
		break;
	case DOWN:
		ghost->objData.Coord.y += 1;
		ghost->objData.preMove = DOWN;
		break;
	case LEFT:
		ghost->objData.Coord.x -= 1;
		ghost->objData.preMove = LEFT;
		break;
	case RIGHT:
		ghost->objData.Coord.x += 1;
		ghost->objData.preMove = RIGHT;
		break;
	default:
		break;
	}
	ghost->objData.facing = ghost->objData.preMove;
	ghost->objData.moveCD = GAME_TICK_CD;
}


static void ghost_stop_script(Ghost* ghost){
	ghost_NextMove(ghost,NONE);
}

static void ghost_move_script_FREEDOM_random(Ghost* ghost, Map* M) {
	// TODO-HACKATHON 2-4: Uncomment the following code and finish pacman picking random direction.
	// hint: see generateRandomNumber in utility.h

	
	// static Directions proba[4]; // possible movement
	// int cnt = 0;
	// for (Directions i = 1; i <= 4; i++)
	// 	if (ghost_movable(ghost,M,i,false)) 	proba[cnt++] = i;
	// ghost_NextMove(ghost, proba[generateRandomNumber(0,cnt-1)]);
	
	// Finish
	// TODO-GC-random_movement: (Not in Hackathon) 
	// Description:
	// For red(Blinky) ghost, we ask you to implement an random strategy ghost, 
	// which means moving in random direction.
	// But your random strategy have to designed carefully so that ghost won't walk back and forth.
	// (The code above DO perform walking back and forth.)
	// Replace the above code by finish followings.
	// hint: record the previous move, and skip it when adding direction into array proba
	
	Directions counter_one = RIGHT;
	switch(ghost->objData.preMove) {
		case RIGHT:
			counter_one = LEFT;
			break;
		case LEFT:
			counter_one = RIGHT;
			break;
		case UP:
			counter_one = DOWN;
			break;
		case DOWN:
			counter_one = UP;
			break;
	}

	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++)
		if (i != counter_one && ghost_movable(ghost,M,i,true)) 	proba[cnt++] = i;
	if (cnt >= 1) {
		ghost_NextMove(ghost, proba[generateRandomNumber(0,cnt-1)]);
	}
	else { // for the dead end case
		ghost_NextMove(ghost, NONE);
	}
	ghost->objData.preMove = ghost->objData.nextTryMove;
}

static void ghost_move_script_FREEDOM_shortest_path(Ghost* ghost, Map* M, Pacman* pman)
{
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pman->objData.Coord.x, pman->objData.Coord.y);
}

static void ghost_move_script_BLOCKED(Ghost* ghost, Map* M) {

	switch (ghost->objData.preMove)
	{
	case UP:
		if (ghost->objData.Coord.y == 10)
			ghost_NextMove(ghost, DOWN);
		else
			ghost_NextMove(ghost, UP);
		break;
	case DOWN:
		if (ghost->objData.Coord.y == 12)
			ghost_NextMove(ghost, UP);
		else
			ghost_NextMove(ghost, DOWN);
		break;
	default:
		ghost_NextMove(ghost, UP);
		break;
	}
}

static void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);	
}

static void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') 
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}

static void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	//ghost is not running away from pacman, maybe
	// TODO-PB: escape from pacman
	// Description:
	// The concept here is to simulate ghosts running away from pacman while pacman is having power bean ability.
	// To achieve this, think in this way. We first get the direction to shortest path to pacman, call it K (K is either UP, DOWN, RIGHT or LEFT).
	// Then we choose other available direction rather than direction K.
	// In this way, ghost will escape from pacman.
	Directions counter_one = RIGHT;
	switch(ghost->objData.preMove) {
		case RIGHT:
			counter_one = LEFT;
			break;
		case LEFT:
			counter_one = RIGHT;
			break;
		case UP:
			counter_one = DOWN;
			break;
		case DOWN:
			counter_one = UP;
			break;
	}

	Directions counter_pman = RIGHT;
	switch (shortestDirection)
	{
		case RIGHT:
			counter_pman = LEFT;
			break;
		case LEFT:
			counter_pman = RIGHT;
			break;
		case UP:
			counter_pman = DOWN;
			break;
		case DOWN:
			counter_pman = UP;
			break;
	}

	static Directions proba[4]; // possible movement
	int cnt = 0;
	for (Directions i = 1; i <= 4; i++){
		if (i!=counter_one && ghost_movable(ghost,M,i,true) && i!= shortestDirection) proba[cnt++] = i;
		else if(i == counter_one && i == counter_pman) proba[cnt++] = i;
	}
	if (cnt >= 1) {
		ghost_NextMove(ghost, proba[generateRandomNumber(0,cnt-1)]);
	}
	else { // for the dead end case
		ghost_NextMove(ghost, shortestDirection);
	}
	ghost->objData.preMove = ghost->objData.nextTryMove;
}

void ghost_move_script_random(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
	else if(get_ghost_stop()){
		ghost_stop_script(ghost);
		return;
	}
	else if(getMode() && ghost->typeFlag == get_ghost_control_index()){
		ghost_move(ghost,M);
		return;
	}
		

		switch (ghost->status)
		{
		case BLOCKED:
			ghost_move_script_BLOCKED(ghost, M);
			if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
				ghost->status = GO_OUT;
			break;
		case FREEDOM:
			ghost_move_script_FREEDOM_random(ghost, M);
			break;
		case GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				ghost->status = BLOCKED;
				ghost->speed = 2; // reset the speed after back to the cage.
				ghost->go_in_time = al_get_timer_count(game_tick_timer); 
			}
			break;
		case FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}
static int mode_change = 0;
void ghost_move_script_shortest_path(Ghost* ghost, Map* M, Pacman* pacman) {
	// TODO-GC-movement: do a little modification on shortest path move script
	// Since always shortest path strategy is too strong, player have no chance to win this.
	// hint: Do shortest path sometime and do random move sometime.
	// Finish
	if (!movetime(ghost->speed))
		return;
	else if(get_ghost_stop()){
		ghost_stop_script(ghost);
		return;
	}
	else if(getMode() && ghost->typeFlag == get_ghost_control_index()){
		ghost_move(ghost,M);
		return;
	}

		switch (ghost->status)
		{
			case BLOCKED:
				ghost_move_script_BLOCKED(ghost, M);
				if (al_get_timer_count(game_tick_timer) - ghost->go_in_time > GO_OUT_TIME)
					ghost->status = GO_OUT;
				break;
			case FREEDOM:
				mode_change++;
				if(mode_change <= 16)
					ghost_move_script_FREEDOM_shortest_path(ghost, M, pacman);
				else if(mode_change>16 && mode_change <= 32)
					ghost_move_script_FREEDOM_random(ghost,M);
				else mode_change = 0;
				break;
			case GO_OUT:
				ghost_move_script_GO_OUT(ghost, M);
				break;
			case GO_IN:
				ghost_move_script_GO_IN(ghost, M);
				if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
					ghost->status = BLOCKED;
					ghost->speed = 2; // reset the speed after back to the cage.
					ghost->go_in_time = al_get_timer_count(game_tick_timer); 
				}
				break;
			case FLEE:
				ghost_move_script_FLEE(ghost, M, pacman);
				break;
			default:
				break;
		}
	

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;

		switch (ghost->objData.preMove) {
		case RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case UP:
			ghost->objData.Coord.y -= 1;
			break;
		case DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}