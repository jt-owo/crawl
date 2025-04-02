#ifndef _GUI_H
#define _GUI_H

#include "game.h"

/* Initializes the graphical user interface with ncurses. */
void gui_init(void);
/* Draws the graphical user interface. */
void gui_draw(Game* g);
/* Draws an object relative to the camera. */
void gui_draw_obj_relative(Point p, char c);
/* Checks if a point is inside the screen bounds. */
bool gui_is_onscr(Point p);
/* Closes the graphical user interface. */
void gui_end(void);

#endif