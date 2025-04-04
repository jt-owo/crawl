#include "cam.h"
#include "gui.h"
#include "calc.h"
#include <ncurses.h>

Point g_cam = {0};

void cam_move(Game* g, enum Direction d)
{
    int stepX = MAX(COLS / 3, 1);
    int stepY = MAX(LINES / 3, 1);

    switch (d)
    {
        case NORTH:
            g_cam.y -= stepY;
            break;
        case SOUTH:
            g_cam.y += stepY;
            break;
        case EAST:
            g_cam.x += stepX;
            break;
        case WEST:
            g_cam.x -= stepX;
        default: break;
    }

    gui_redraw(g);
}

void cam_center(Point p)
{
    g_cam.x = p.x - COLS / 2;
    g_cam.y = p.y - LINES / 2;
}