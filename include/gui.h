#ifndef _GUI_H
#define _GUI_H

#include "game.h"
#include "color.h"
#include "list.h"

/* Initializes the graphical user interface with ncurses. */
void gui_init(void);
/* Draws the graphical user interface. */
void gui_draw(Game* g);
/* Draws the game info. */
void gui_draw_info(Game* g);
/* Redraws the graphical user interface depending on the game state. */
void gui_redraw(Game* g);
/* Draws an object relative to the camera. */
void gui_draw_obj_relative(Point p, char c);
/* Draws an object based on visibility. */
void gui_draw_obj_conditionally(Level* l, Point p, char c, enum Color color);
/* Draws an object as an overlay. */
void gui_draw_as_overlay(Point p, char c, enum Color color);
/* Draws an selector for cam mode. */
void gui_draw_cam_selector();
/* Sets the status and redraws it */
void gui_status(const char* msg);
/* Displays an alert with a message. */
void gui_alert(const char* msg);
/* Displays a confirm dialogue. */
bool gui_confirm(const char* msg, char a, char b);
/* Checks if a point is inside the screen bounds. */
bool gui_is_onscr(Point p);
/* Closes the graphical user interface. */
void gui_end(void);

#endif