#ifndef _CALC_H
#define _CALC_H

#define NOISE_CHAR '#'

#define MAX(x, y) ((x > y) ? x : y)

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

typedef struct Rect
{
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct Point
{
    int x;
    int y;
} Point;

void noise();

#endif