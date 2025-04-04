#ifndef _CALC_H
#define _CALC_H

#define NOISE_CHAR '#'

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)
#define LENGTH(X) (sizeof X / sizeof X[0])
#define ABS(x) ((x < 0) ? -x : x)

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

/* Returns the amount of digits an integer has. INTEGER OPTIMIZED CODE! */
int digits(int n);
void noise();

#endif