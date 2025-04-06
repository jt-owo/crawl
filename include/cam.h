#ifndef _CAM_H
#define _CAM_H

#include "game.h"
#include "calc.h"

/* Moves the camera. */
void cam_move(Game *g, enum Direction d);
/* Centers the cam around a specific point. */
void cam_center(Point p);
/* Moves the camera selector. */
void cam_move_selector(Game* g, enum Direction d);

/* Global camera */
extern Point g_cam;

/* Global selector */
extern Point g_selector;

#endif