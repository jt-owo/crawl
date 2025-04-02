#ifndef _CAM_H
#define _CAM_H

#include "game.h"
#include "calc.h"

/* Moves the camera in d direction. */
void cam_move(Game *g, enum Direction d);

/* Global camera */
extern Point g_cam;

#endif