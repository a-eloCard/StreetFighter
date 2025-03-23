#ifndef __PUNCH__
#define __PUNCH__

#include <stdio.h>
#include <stdlib.h>

#define PUNCH_SIDE 50
#define PUNCH_COOLDOWN 20

typedef struct{
    unsigned char side_x;
    unsigned char side_y;
    unsigned short x;
    unsigned short y;
} punch;

punch *punch_create (unsigned char side_x, unsigned char side_y, unsigned short x, unsigned short y);

void punch_destroy (punch *element);

#endif