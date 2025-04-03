#ifndef _GUI_H
#define _GUI_H

#include "game.h"

/* Initializes the graphical user interface with ncurses. */
void gui_init(void);
/* Draws the graphical user interface. */
void gui_draw(Game* g);
/* Redraws the graphical user interface depending on the game state. */
void gui_redraw(Game* g);
/* Draws an object relative to the camera. */
void gui_draw_obj_relative(Point p, char c);
/* Displays an alert with a message. */
void gui_alert(const char* msg);
/* Displays a confirm dialogue. */
bool gui_confirm(const char* msg, char a, char b);
/* Checks if a point is inside the screen bounds. */
bool gui_is_onscr(Point p);
/* Closes the graphical user interface. */
void gui_end(void);

#endif