#ifndef _GAME_H
#define _GAME_H

#include "level.h"
#include "player.h"
#include "calc.h"

enum GameState {
    MAP_WALK,
    INFO_SCREEN
};

typedef struct Game
{
    enum GameState      state;
    Player*             player;
    Level*              levels;
    Level*              current;
    bool                running;
    int                 floor;
} Game;

/* Creates a new game and allocates the memory. */
Game* game_new(void);
/* Frees the memory of a game */
void game_free(Game* g);
/* Main game loop. */
void game_run(Game* g);
/* Renders the game info. */
void game_render_info(Game* g);
/* Updates the game state. */
void game_state(Game* g, enum GameState state);
/* Ascends a level up. */
void game_ascend(Game* g);
/* Descends a level down. */
void game_descend(Game* g);
/* Move a player in a direction. */
void game_move(Game* g, enum Direction d);
/* Handle control input. */
void handle_input(Game* g, int key);

#endif