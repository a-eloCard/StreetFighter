#include <stdlib.h>
#include "Square.h"

state *state_create(){
    state *new_state = (state*) malloc(sizeof(state));
	if (!new_state) return NULL;

	new_state->jump = 0;
	new_state->colidiu = 0;	
	new_state->agacha = 0;
	new_state->punch = 0;
	new_state->kick = 0;

	return new_state;
}

square *square_create(unsigned char side_x, unsigned char side_y, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){

    if ((x - side_x/2 < 0) || (x + side_x/2 > max_x) || (y - side_y/2 < 0) || (y + side_y/2 > max_y)) return NULL;										
	if (face > 1) return NULL;

	square *new_square = (square*) malloc(sizeof(square));
	if (!new_square) return NULL;																		

	unsigned short x_punch;
	unsigned short x_kick;
	new_square->side_x = side_x;
	new_square->side_y = side_y;	
	new_square->face = face;																										
	new_square->x = x;																									
	new_square->y = y;			
	new_square->vly = 0;
	new_square->hp = HP;
	new_square->timer = 0;
	new_square->pause_timer = 0;
	new_square->quadro = PARADO;
	new_square->acao = state_create();
	if (!face){
		x_punch = x - side_x/2; 
		x_kick = x - side_x/2; 
	}
	else {
		x_punch= x + side_x/2;
		x_kick = x + side_x/2;
	}
	new_square->punch = punch_create(0, 0, x_punch, y - side_y/2 + 5);
	new_square->kick = punch_create(0, 0, x_kick, y + side_y/2 - 5);
	new_square->control = joystick_create();																			
	return new_square;														
}

void movimentacao (ALLEGRO_EVENT event, square *player_1, square *player_2){
	if (event.keyboard.keycode == ALLEGRO_KEY_A)
		joystick_left(player_1->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_D){
		if ((event.type == 10 && !player_1->control->right) || (event.type == 12 && player_1->control->right))
			joystick_right(player_1->control);

	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_S){
		joystick_down(player_1->control);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && !player_1->control->left && !player_1->control->right){
			player_1->acao->agacha = 1;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
			player_1->acao->agacha = 0;
	}
	
	if (event.keyboard.keycode == ALLEGRO_KEY_W)
		joystick_up(player_1->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_E && player_1->pause_timer == 0 && !player_1->acao->kick && !player_1->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_punch(player_1->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F && player_1->pause_timer == 0 && !player_1->acao->punch && !player_1->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_kick(player_1->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_Q && !player_1->acao->kick && !player_1->acao->punch /*&& !player_1->control->left && !player_1->control->right*/)
		joystick_parry(player_1->control);
	
	if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && !player_2->acao->agacha)
		joystick_left(player_2->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && !player_2->acao->agacha)
		joystick_right(player_2->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN){
		joystick_down(player_2->control);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN  && !player_2->control->left && !player_2->control->right){
			player_2->acao->agacha = 1;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
			player_2->acao->agacha = 0;
	}

	if (event.keyboard.keycode == ALLEGRO_KEY_UP){
		joystick_up(player_2->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1 && player_2->pause_timer == 0 && !player_2->acao->kick && !player_2->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_punch(player_2->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_0 && player_2->pause_timer == 0 && !player_2->acao->punch && !player_2->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_kick(player_2->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2 && !player_2->acao->kick && !player_2->acao->punch && !player_2->control->left && !player_2->control->right){
		joystick_parry(player_2->control);
	}

}

void movimentacao_single(ALLEGRO_EVENT event, square *player_1){
	if (event.keyboard.keycode == ALLEGRO_KEY_A)
		joystick_left(player_1->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_D){
		if ((event.type == 10 && !player_1->control->right) || (event.type == 12 && player_1->control->right))
			joystick_right(player_1->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_S){
		joystick_down(player_1->control);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && !player_1->control->left && !player_1->control->right){
			player_1->acao->agacha = 1;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP)
			player_1->acao->agacha = 0;
	}
	
	if (event.keyboard.keycode == ALLEGRO_KEY_W)
		joystick_up(player_1->control);
	else if (event.keyboard.keycode == ALLEGRO_KEY_E && player_1->pause_timer == 0 && !player_1->acao->kick && !player_1->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_punch(player_1->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_F && player_1->pause_timer == 0 && !player_1->acao->punch && !player_1->control->parry){
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			joystick_kick(player_1->control);
	}
	else if (event.keyboard.keycode == ALLEGRO_KEY_Q && !player_1->acao->kick && !player_1->acao->punch /*&& !player_1->control->left && !player_1->control->right*/)
		joystick_parry(player_1->control);

	
}

void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x){							//Implementação da função "square_move" (-1)
	//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	if (!trajectory){ 
		if ((element->x - steps*SQUARE_STEP) - element->side_x/2 >= 0) 
			element->x = element->x - steps*SQUARE_STEP;
	} 	
	//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma			
	else if (trajectory == 1){ 
		if ((element->x + steps*SQUARE_STEP) + element->side_x/2 <= max_x) 
			element->x = element->x + steps*SQUARE_STEP;
	}	
}

float calcular_distancia(square *player_1, square *player_2) {
    float player1_esquerda = player_1->x - player_1->side_x / 2;
    float player1_direita = player_1->x + player_1->side_x / 2;
    float player2_esquerda = player_2->x - player_2->side_x / 2;
    float player2_direita = player_2->x + player_2->side_x / 2;

    if (player1_direita < player2_esquerda)
        return player2_esquerda - player1_direita;
    return player1_esquerda - player2_direita;
}

void square_punch (square *element, square *adversario){
	if (element->acao->jump){
		int distancia = (int)calcular_distancia(element, adversario);
		if (distancia <= 30){
			if((adversario->x < element->x && !element->face) || (adversario->x > element->x && element->face)){
				if (adversario->control->parry)
					return;
				adversario->hp-=5;
				return;
			}
		}
	}
	else {
		if (((element->punch->x - element->punch->side_x/2) <= (adversario->x + adversario->side_x/2)) && 
			((element->punch->y - element->punch->side_y/2) <= (adversario->y + adversario->side_y/2)) && //
			((element->punch->x + element->punch->side_x/2) >= (adversario->x - adversario->side_x/2)) && //
			((element->punch->y + element->punch->side_y/2) >= (adversario->y - adversario->side_y/2))){
			if (adversario->hp){
				if (adversario->control->parry){
					if ((((element->punch->x - element->punch->side_x/2) <= (adversario->x + adversario->side_x/2)) && 
						((element->punch->y - element->punch->side_y/2) <= ((adversario->y + adversario->side_y/4) + adversario->side_y/4)) && //
						((element->punch->x + element->punch->side_x/2) >= (adversario->x - adversario->side_x/2)) && //
						((element->punch->y + element->punch->side_y/2) >= ((adversario->y + adversario->side_y/4) - adversario->side_y/4))) || (element->face == adversario->face)){
							adversario->hp-=5;
					}
					else return;
				}
				else adversario->hp-= 5;
				return;
			}
		}
	}
}

void square_kick (square *element, square *adversario){
	if (((element->kick->x - element->kick->side_x/2) <= (adversario->x + adversario->side_x/2)) && 
		((element->kick->y - element->kick->side_y/2) <= (adversario->y + adversario->side_y/2)) && //
		((element->kick->x + element->kick->side_x/2) >= (adversario->x - adversario->side_x/2)) && //
		((element->kick->y + element->kick->side_y/2) >= (adversario->y - adversario->side_y/2))){
		if (adversario->hp){
			if (adversario->control->parry){
				if (element->acao->agacha || (element->face == adversario->face)){
						adversario->hp-= 5;
						return;
				}
				else return;
			}
			else adversario->hp -= 5;
			return;
		}
	}
}

void square_destroy(square *element){	
	punch_destroy(element->punch);
	punch_destroy(element->kick);																										//Implementação da função "square_destroy"
	joystick_destroy(element->control);																											//Destrói controle do quadrado (!)
	state_destroy(element->acao);
	free(element);																																//Libera a memória do quadrado na heap
}

void reset_punch_kick (square *player){
    unsigned short x_punch;
	unsigned short x_kick;
    if (!player->face){
		x_punch = player->x - player->side_x/2; 
		x_kick = player->x - player->side_x/2; 
	}
	else {
		x_punch= player->x + player->side_x/2;
		x_kick = player->x + player->side_x/2;
	}
    player->punch->side_x = 0;
    player->punch->side_y = 0;
    player->punch->x = x_punch;
    player->punch->y = player->y - player->side_y/2 + 5;

    player->kick->side_x = 0;
    player->kick->side_y = 0;
    player->kick->x = x_kick;
    player->kick->y = player->y - player->side_y/2 + 5;
}


void state_destroy(state *element){
	free(element);
}