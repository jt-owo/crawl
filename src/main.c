#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "gui.h"
#include "game.h"

Game *g = NULL;

static void interrupt_handler(int sign)
{
    g->running = !gui_confirm("Do you want to force quit? (safely)", 'y', 'n');
    if (g->running)
    {
        gui_redraw(g);
    }
    else
    {
        gui_end();
        exit(EXIT_SUCCESS);
    }
    
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    gui_init();

    signal(SIGINT, interrupt_handler);

    g = game_new();
    gui_draw(g);
    game_run(g);
    game_free(g);
    
    gui_end();

    return 0;
}