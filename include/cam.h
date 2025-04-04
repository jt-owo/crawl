#ifndef _CAM_H
#define _CAM_H

#include "game.h"
#include "calc.h"

/* Moves the camera in d direction. */
void cam_move(Game *g, enum Direction d);
/* Centers the cam around a specific point. */
void cam_center(Point p);

/* Global camera */
extern Point g_cam;

#endif