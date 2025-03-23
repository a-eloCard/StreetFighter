#include "Attack.h"



punch *punch_create (unsigned char side_x, unsigned char side_y, unsigned short x, unsigned short y){
    punch *new_punch = (punch *)malloc(sizeof(punch));
    if (!new_punch) return NULL;

    new_punch->side_x = side_x;
    new_punch->side_y = side_y;
    new_punch->x = x;
    new_punch->y = y;
    return new_punch;
}

void punch_destroy (punch *element){
    free(element);
}