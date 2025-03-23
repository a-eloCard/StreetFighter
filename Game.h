#ifndef __GAME__																											//Guardas de inclusão
#define __GAME__

#include "Square.h"

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro5.h>	
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>	

#define X_SCREEN 800
#define Y_SCREEN 550
#define X_SIDE 50
#define Y_SIDE 160
#define CHAO 490
#define LARGURA_SPRITE 100

typedef struct{
    ALLEGRO_BITMAP *sprite;
    unsigned int largura;
    unsigned short altura;
    unsigned char frames;
    unsigned char posicao;
} movimento;

movimento **cria_personagem (unsigned char tamanho);

void inicializa_personagem (movimento **vetor, unsigned char tamanho, const char *jogador);

unsigned char collision_2D(square *player_2_first, square *player_2_second);

float calcular_dist_bot(square *bot, square *p);

void update_position(square *player_1, square *player_2, bool single_player);

void update_kick (square *element);

void update_punch (square *element);

void check_kill (square *player_1, square *player_2, unsigned char *p1k, unsigned char *p2k, int *p1_round_won, int *p2_round_won);

void liberar_personagens(movimento **vetor, unsigned char tamanho);

void draw_sprites(square *player);

void acao_single_player(square *bot, square *player);

#endif