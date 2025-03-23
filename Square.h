#ifndef __SQUARE__ 																												//Guardas de inclusão
#define __SQUARE__																												//Guardas de inclusão																										//Quantidade de pixels que um quadrado se move por passo
#include <allegro5/allegro5.h>	
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_primitives.h>	
#include <math.h>
#include "Joystick.h"
#include "Attack.h"
#include <stdio.h>																											//Estrutura e procedimentos relacionados ao controle do quadrado (!)

#define SQUARE_STEP 5																										//Tamanho, em pixels, de um passo do quadrado
#define GRAVITY 3
#define HP 100																							

enum tipos_acoes{
    PARADO, ANDANDO, ABAIXADO, CHUTE, CHUTE_AGACHADO,
    CHUTE_PULANDO, DEFESA_PE, DEFESA_ABAIXADO, PULO, SOCO, SOCO_AGACHADO,
    SOCO_PULANDO, VITORIA1, VITORIA2
};

typedef struct {
	unsigned short jump;
	unsigned short colidiu;
	unsigned short agacha;
	unsigned short punch;
	unsigned short kick;
} state;

typedef struct {		
	unsigned char face;	
	unsigned char side_x;
	unsigned char side_y;			
	unsigned short x;		
	unsigned short y;
	char vly;
	int hp;
	unsigned short timer;
	unsigned short pause_timer;
	enum tipos_acoes quadro;
	state *acao;
	punch *kick;
	punch *punch;
	joystick *control;																											//Elemento de controle do quadrado no jogo (!)	
} square;																														//Definição do nome da estrutura


state *state_create();

square* square_create(unsigned char side_x, unsigned char side_y, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado

void movimentacao (ALLEGRO_EVENT event, square *player_1, square *player_2);

void movimentacao_single(ALLEGRO_EVENT event, square *player_1);

void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x);

float calcular_distancia(square *player_1, square *player_2);

void square_punch (square *element, square *adversario);

void square_kick (square *element, square *adversario);

void reset_punch_kick (square *player);

void square_destroy(square *element);

void state_destroy(state *element);																					//Protótipo da função de destruição de um quadrado

#endif																															//Guardas de inclusão