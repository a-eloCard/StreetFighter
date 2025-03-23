CC = gcc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5)

LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5)

all: street_fighter

street_fighter: aggressiveSquares.c Square.c Joystick.c Game.c Attack.c 
	$(CC) $(CFLAGS) aggressiveSquares.c Square.c Joystick.c Game.c Attack.c -o street_fighter $(LDFLAGS) -lm
clean:
	-rm -f *.o 

purge: clean
	-rm -f street_fighter
	-rm -f *.swp
.PHONY: all clean purge
