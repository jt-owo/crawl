#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "gui.h"
#include "game.h"

int main(int argc, char** argv)
{
    srand(time(NULL));

    gui_init();

    Game* g = game_new();
    gui_draw(g);
    game_run(g);
    game_free(g);
    
    gui_end();

    return 0;
}