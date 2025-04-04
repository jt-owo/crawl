#ifndef _CALC_H
#define _CALC_H

#include <tile.h>

#define NOISE_CHAR '#'

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)
#define LENGTH(X) (sizeof X / sizeof X[0])
#define ABS(x) ((x < 0) ? -x : x)

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NW,
    NE,
    SW,
    SE
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

/* Checks if two points are equal. */
bool pteq(Point p1, Point p2);

/* Returns the amount of digits an integer has. INTEGER OPTIMIZED CODE! */
int digits(int n);
void noise();

#endif