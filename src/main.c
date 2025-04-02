#include <ncurses.h>

#include "gui.h"
#include "game.h"

int main(int argc, char** argv)
{
    gui_init();

    Game* g = game_new();
    game_render(g);
    game_run(g);
    
    gui_end();

    return 0;
}