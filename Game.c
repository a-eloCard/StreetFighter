#include "Game.h"

movimento **cria_personagem (unsigned char tamanho){
    movimento **vetor = (movimento **)malloc(tamanho * sizeof(movimento *));

	if (!vetor) return NULL;

	for (int i = 0; i < tamanho; i++){
		vetor[i] = (movimento *)malloc(sizeof(movimento));
		if (!vetor[i]) {
            for (int j = 0; j < i; j++)
                free(vetor[j]);
			free(vetor);
			return NULL;
		}
		vetor[i]->sprite = NULL;
		vetor[i]->largura = 0;
		vetor[i]->altura = 0;
		vetor[i]->frames = 0;
		vetor[i]->posicao = 0;
	}

	return vetor;
}

void inicializa_personagem (movimento **vetor, unsigned char tamanho, const char *jogador){
	char caminho[256]; // Vai juntar as palavras
	
	const char *imagens[] = {
		"parado.png", "andando.png", "abaixado.png", "chute.png", "chute_agachado.png", "chute_pulando.png",
		"defesa_pe.png", "defesa_abaixado.png", "pulo.png", "soco.png", "soco_agachado.png",
		"soco_pulando.png", "vitoria1.png", "vitoria2.png"
	};

	for(int i = 0; i < tamanho; i++){
		snprintf(caminho, sizeof(caminho), "./personagens/%s/%s", jogador, imagens[i]);
		vetor[i]->sprite = al_load_bitmap(caminho);
		if (!vetor[i]->sprite){
			for (int j = 0; j < i; j++) {
                if (vetor[j]->sprite) {
                    al_destroy_bitmap(vetor[j]->sprite);
                }
                free(vetor[j]);
			}
			free(vetor);
			return;
		}
	}
	for(int i = 0; i < tamanho; i++){
		vetor[i]->largura = al_get_bitmap_width(vetor[i]->sprite);
		vetor[i]->altura = al_get_bitmap_height(vetor[i]->sprite);
		vetor[i]->frames = al_get_bitmap_width(vetor[i]->sprite) / 100;
	}
}

unsigned char collision_2D(square *player_1, square *player_2)
{
	float esquerdo_1 = player_1->x - player_1->side_x / 2;
	float direito_1 = player_1->x + player_1->side_x / 2;
	float cima_1 = player_1->y - player_1->side_y / 2;
	float baixo_1 = player_1->y + player_1->side_y / 2;

	float esquerdo_2 = player_2->x - player_2->side_x / 2;
	float direito_2 = player_2->x + player_2->side_x / 2;
	float cima_2 = player_2->y - player_2->side_y / 2;
	float baixo_2 = player_2->y + player_2->side_y / 2;

    if ((((baixo_1 > cima_2) && (cima_2 > cima_1)) ||				  //			//Verifica se o primeiro player_2o colidiu com o segundo no eixo X
		 ((baixo_2 >= cima_1) && (cima_1 >= cima_2))) &&			  //			//Verifica se o segundo player_2o colidiu com o primeiro no eixo X
		(((direito_1 >= esquerdo_2) && (esquerdo_2 >= esquerdo_1)) || //			//Verifica se o primeiro player_2o colidiu com o segundo no eixo Y
		 ((direito_2 >= esquerdo_1) && (esquerdo_1 >= esquerdo_2)))	  //			//Verifica se o segundo player_2o colidiu com o primeiro no eixo Y
		 ) return 1;
		return 0;
}

float calcular_dist_bot(square *bot, square *p) {
    int dx = p->x - bot->x;
    int dy = p->y - bot->y;
    return sqrt(dx * dx + dy * dy);
}

void update_position(square *player_1, square *player_2, bool single_player){																																				//Função de atualização das posições dos quadrados conforme os comandos do controle (!)
	if (player_1->control->left && !player_1->acao->agacha && !player_1->acao->punch && !player_1->acao->kick){
		/*if (player_1->control->parry) player_1->control->parry = 0;*/
		square_move(player_1, 2, 0, X_SCREEN);																																				//Move o quadrado do primeiro jogador para a esquerda (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -2, 0, X_SCREEN);
		player_1->face = 0;																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	
	if (player_1->control->right && !player_1->acao->agacha && !player_1->acao->punch && !player_1->acao->kick){	
		/*if (player_1->control->parry) player_1->control->parry = 0;		*/																																							//Se o botão de movimentação para direita do controle do primeir ojogador está ativado... (!)
		square_move(player_1, 2, 1, X_SCREEN);																																				//Move o quadrado do primeiro jogador para a direta (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -2, 1, X_SCREEN);	
		player_1->face = 1;																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}

	/*PULO DO PLAYER 1*/
	if (player_1->control->up && player_1->acao->jump == 0 && !player_1->control->down && !player_1->acao->agacha){																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		player_1->vly = -40;
		player_1->acao->jump = 1;
	}

	if (player_1->acao->jump == 1){
		if (player_1->acao->colidiu == 0){
			player_1->vly += GRAVITY;
			player_1->y += player_1->vly;

			if (player_1->y >= CHAO-player_1->side_y/2){
				player_1->y = CHAO-player_1->side_y/2;
				player_1->vly = 0;
				player_1->acao->jump = 0;
			}
		}

		if (collision_2D(player_2, player_1)){
			player_1->acao->colidiu = 1;
			player_1->y = CHAO - player_2->side_y - player_1->side_y/2 - 1;
			player_1->acao->jump = 0;
			if (player_1->x > player_2->x  && (player_1->x+player_1->side_x/2 < X_SCREEN - 1))
				player_1->x += 7;
			else if (player_1->x-player_1->side_x/2 > 0)
				player_1->x -= 7;
		}
	}
	if (player_1->acao->colidiu){
		if (!collision_2D(player_2, player_1)){
			player_1->acao->colidiu = 0;
			player_1->acao->jump = 1;
		}
	}

		/* AGACHAMENTO DO PLAYER 1*/
	bool agacha_1 = true;
	if (player_1->acao->agacha){
		if (player_1->y == (CHAO - player_2->side_y - player_1->side_y/2 - 1)){
			player_1->y = CHAO - player_2->side_y -player_1->side_y/2 -1;
			agacha_1 = false;
		}
		if (player_1->y < (CHAO - player_1->side_y/2)){
			agacha_1 = false;
		}
		if(agacha_1){
			player_1->side_y = Y_SIDE / 2;
			player_1->y = CHAO - player_1->side_y/2;
			if (collision_2D(player_1, player_2)){
				if (player_2->y+player_2->side_y/2 >= player_1->y-player_1->side_y/2)
					player_2->y = CHAO - player_1->side_y - player_2->side_y/2;
			}
		}
	}
	else if (!player_1->acao->jump){
		player_1->side_y = Y_SIDE;
		player_1->y = CHAO - player_1->side_y/2;
		if (collision_2D(player_1, player_2))
			player_2->y = CHAO - player_1->side_y - player_2->side_y/2 - 1;
	}
	// IMPLEMENTAÇÃO DO SINGLE PLAYER
	if (single_player){
		if (((player_2->x-(player_2->side_x/2)) > (player_1->x + (player_1->side_x/2))) && ((calcular_dist_bot(player_2, player_1)) >= 20.0)){	
			player_2->control->left = 1;																																			//Move o quadrado do segundo jogador para a esquerda (!)
			player_2->face = 0;																									
			square_move(player_2, 1, 0, X_SCREEN);
			if (collision_2D(player_1, player_2)) {
				player_2->control->left = 0;
				player_2->control->right = 0;
				player_2->quadro = PARADO;
				square_move(player_2, -1, 0, X_SCREEN);
			}
		}
		else if (((player_2->x+(player_2->side_x/2)) < (player_1->x - (player_1->side_x/2)))  && ((calcular_dist_bot(player_2, player_1)) >= 20.0)){ 
			player_2->control->right = 1;			
			player_2->face = 1;																										
			square_move(player_2, 1, 1, X_SCREEN);																																				//Move o quadrado do segundo jogador para a direita (!)
			if (collision_2D(player_1, player_2)) {
				player_2->control->right = 0;
				player_2->control->left = 0;
				player_2->quadro = PARADO;
				square_move(player_2, -1, 1, X_SCREEN);	
			}
		}
	}
	else {
		if (player_2->control->left  && !player_2->acao->agacha && !player_2->acao->punch && !player_2->acao->kick){	
			if (player_2->control->parry) player_2->control->parry = 0;																																									//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
			square_move(player_2, 2, 0, X_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
			if (collision_2D(player_2, player_1)) square_move(player_2, -2, 0, X_SCREEN);	
			player_2->face = 0;																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		}
		
		if (player_2->control->right  && !player_2->acao->agacha && !player_2->acao->punch && !player_2->acao->kick){ 			
			if (player_2->control->parry) player_2->control->parry = 0;																																							//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
			square_move(player_2, 2, 1, X_SCREEN);																																				//Move o quadrado do segundo jogador para a direita (!)
			if (collision_2D(player_2, player_1)) square_move(player_2, -2, 1, X_SCREEN);	
			player_2->face = 1;																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
		}
	}
	
	
	/* PULO DO PLAYER 2*/
	if (player_2->control->up && player_2->acao->jump == 0 && !player_2->control->down && !player_2->acao->agacha){																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		player_2->vly = -40;
		player_2->acao->jump = 1;
		if(single_player){
			player_2->control->up = 0;
		}
	}

	if (player_2->acao->jump == 1 ){
		if (player_2->acao->colidiu == 0){
			player_2->vly += GRAVITY;
			player_2->y += player_2->vly;

			if (player_2->y >= CHAO-player_2->side_y/2){
				player_2->y = CHAO-player_2->side_y/2;
				player_2->vly = 0;
				player_2->acao->jump = 0;
			}
		}
		if (collision_2D(player_2, player_1)){
				player_2->acao->colidiu = 1;
				player_2->y = CHAO - player_1->side_y - player_2->side_y/2 - 1;
				player_2->acao->jump = 0;
				if (player_2->x > player_1->x  && (player_2->x+player_2->side_x/2 < X_SCREEN - 1))
					player_2->x += 7;
				else if (player_2->x-player_2->side_x/2 > 0)
					player_2->x -= 7;
		}
	}

	if (player_2->acao->colidiu){
		if (!collision_2D(player_2, player_1)){
			player_2->acao->colidiu = 0;
			player_2->acao->jump = 1;
		}
	}
	
	/* AGACHAMENTO DO PLAYER 2*/
	bool agacha_2 = true;
	if (player_2->acao->agacha){
		//verifica se esta literalmente em cima do player_1
		if (player_2->y == (CHAO - player_1->side_y - player_2->side_y/2 - 1)){
			player_2->y = CHAO - player_1->side_y -player_2->side_y/2 - 1;
			agacha_2 = false;
		}
		if (player_2->y < (CHAO - player_2->side_y/2)){
			agacha_2 = false;
		}
		if(agacha_2){
			// atualiza posição
			player_2->side_y = Y_SIDE / 2;
			player_2->y = CHAO - player_2->side_y/2;
			if (collision_2D(player_1, player_2)){
				if (player_1->y+player_1->side_y/2 >= player_2->y-player_2->side_y/2)
					player_1->y = CHAO - player_2->side_y - player_1->side_y/2;
			}
		}
	}
	else if (!player_2->acao->jump){
		player_2->side_y = Y_SIDE;
		player_2->y = CHAO - player_2->side_y/2;
		if (collision_2D(player_1, player_2))
			player_1->y = CHAO - player_2->side_y - player_1->side_y/2 - 1;
	}

	/* IMPLEMENTAÇÃO DO SOCO*/

	if (player_1->control->punch && !player_1->control->left && !player_1->control->right){
		if (!player_1->acao->punch){
			player_1->acao->punch = 1;
			player_1->control->punch = 0;
			update_punch(player_1);
			if (player_1->pause_timer == 0){
				square_punch(player_1, player_2);
			}
			if (player_1->acao->jump)
				player_1->timer = 15;
			else 
				player_1->timer = PUNCH_COOLDOWN;
		}
		update_punch(player_1);
	}


	if (player_2->control->punch && !player_2->control->left && !player_2->control->right){
		if (!player_2->acao->punch){
			player_2->acao->punch = 1;
			player_2->control->punch = 0;
			update_punch(player_2);
			if (player_2->pause_timer == 0)
				square_punch(player_2, player_1);
			if (player_2->acao->jump)
				player_2->timer = 15;
			else 
				player_2->timer = PUNCH_COOLDOWN;
		}
		update_punch(player_2);
	}
	//printf("pausa %d\n", player_2->pause_timer);

	/* IMPLEMENTAÇÃO DO CHUTE*/
	if (player_1->control->kick && !player_1->control->left && !player_1->control->right){
		if (!player_1->acao->kick){
			player_1->acao->kick = 1;
			player_1->control->kick = 0;
			update_kick(player_1);
			if (player_1->pause_timer == 0)
				square_kick(player_1, player_2);
			player_1->timer = PUNCH_COOLDOWN;
		}
		update_kick(player_1);
	}

	if (player_2->control->kick && !player_2->control->left && !player_2->control->right){
		if (!player_2->acao->kick){
			player_2->acao->kick = 1;
			player_2->control->kick = 0;
			update_kick(player_2);
			if (player_2->pause_timer == 0)
				square_kick(player_2, player_1);
			player_2->timer = PUNCH_COOLDOWN;
		}
		update_kick(player_2);
	}
	update_punch(player_1);
	update_kick(player_1);
	update_punch(player_2);
	update_kick(player_2);

}

void update_kick(square *element) {

	if (element->acao->agacha){
		element->kick->y = element->y + element->side_y / 2 - 8;
	}
	else{
    	element->kick->y = element->y - element->side_y / 2 + 8;
	}

	if (element->acao->kick){
		if (!element->face) {
			element->kick->x = element->x - element->side_x / 2 - PUNCH_SIDE / 2;
		} else {
			element->kick->x = element->x + element->side_x / 2 + PUNCH_SIDE / 2;
		}
		element->kick->side_x = PUNCH_SIDE;
		element->kick->side_y = 15;
	}
	else {
		element->kick->x = element->x + element->side_x/2;
        element->kick->side_x = 0;
        element->kick->side_y = 0;
	}
}

void update_punch(square *element) {
	if (element->acao->agacha){
    	element->punch->y = element->y - element->side_y / 2 + 8;
	}
	else{
    	element->punch->y = element->y - element->side_y / 2 + 30;
	}
    if (element->acao->punch && !element->acao->jump/*&& (element->timer <= 10)*/){
		if (!element->face) {
			element->punch->x = element->x - element->side_x / 2 - PUNCH_SIDE / 2;
		} else {
			element->punch->x = element->x + element->side_x / 2 + PUNCH_SIDE / 2;
		}
		element->punch->side_x = PUNCH_SIDE;
		element->punch->side_y = 15;
	}
	else if (element->acao->punch && element->acao->jump){
		element->punch->y = element->y + element->side_y / 2 - 8;
		if (!element->face) {
			element->punch->x = element->x - element->side_x / 2 - PUNCH_SIDE / 2;
		} else {
			element->punch->x = element->x + element->side_x / 2 + PUNCH_SIDE / 2;
		}
		element->punch->side_x = PUNCH_SIDE;
		element->punch->side_y = 15;
	}
	else {
		element->punch->x = element->x + element->side_x/2;
		element->punch->y = element->y - element->side_y / 2 + 8;
        element->punch->side_x = 0;
        element->punch->side_y = 0;
		
	}
}


void check_kill (square *player_1, square *player_2, unsigned char *p1k, unsigned char *p2k, int *p1_round_won, int *p2_round_won){
	if(player_1->hp <= 0){
		(*p2k)++;
		(*p2_round_won) = 1;
	}
	if(player_2->hp <= 0){
		(*p1k)++;
		(*p1_round_won) = 1;
	}
	player_1->hp = HP;
	player_1->y = CHAO - (Y_SIDE/ 2);
	player_1->face = 1;
	player_1->timer = 0;
	player_1->pause_timer = 0;
	player_1->quadro = PARADO;
	player_1->acao->jump = 0;
	player_1->acao->colidiu = 0;	
	player_1->acao->agacha = 0;
	player_1->acao->punch = 0;
	player_1->acao->kick = 0;

	player_2->hp = HP;
	player_2->y = CHAO - (Y_SIDE / 2);
	player_2->face = 0;
	player_2->timer = 0;
	player_2->pause_timer = 0;
	player_2->quadro = PARADO;
	player_2->acao->jump = 0;
	player_2->acao->colidiu = 0;	
	player_2->acao->agacha = 0;
	player_2->acao->punch = 0;
	player_2->acao->kick = 0;

	reset_punch_kick(player_1);
	reset_punch_kick(player_2);

}

void liberar_personagens(movimento **vetor, unsigned char tamanho) {
    for (int i = 0; i < tamanho; i++) {
        al_destroy_bitmap(vetor[i]->sprite);
		free(vetor[i]);
    }
    free(vetor);
}

void draw_sprites(square *player){

	if (player->control->left && !player->acao->agacha) {
		if(!player->acao->jump){
			player->quadro = ANDANDO;
		}
		else if (player->acao->jump){
			player->quadro = PULO;
		}
	} 
	else if (player->acao->jump){
		if (!player->acao->punch && !player->acao->kick){
			player->quadro = PULO;
		}
		else if (player->acao->punch){
			player->quadro = SOCO_PULANDO;
		}
		else if (player->acao->kick){
			player->quadro = CHUTE_PULANDO;
		}
	}
	else if (player->control->right && !player->acao->agacha){
		if (!player->acao->jump){
			player->quadro = ANDANDO;
		}
		else if (player->acao->jump){
			player->quadro = PULO;
		}
	}
	else if (player->acao->agacha){
		if (!player->acao->punch && !player->acao->kick && !player->control->parry){
			player->quadro = ABAIXADO;
		}
		else if (player->acao->punch){
			player->quadro = SOCO_AGACHADO;
		}
		else if (player->acao->kick){
			player->quadro = CHUTE_AGACHADO;
		}
		else if (player->control->parry){
			player->quadro = DEFESA_ABAIXADO;
		}
	}
	else if (player->acao->kick){
		player->quadro = CHUTE;
	}
	else if (player->acao->punch){
		player->quadro = SOCO;
	}
	else if (player->control->parry){
		player->quadro = DEFESA_PE;
	}
	else {
		player->quadro = PARADO;
	}
}
// FUNÇOES PARA O BOT
void acao_single_player(square *bot, square *player){	

	int distancia = (int)calcular_distancia(bot, player);
	if (distancia >= 30)
		return;
	
	if (bot->timer == 0){
		bot->control->punch = 0;
		bot->control->kick = 0;
	}

	int ataque_randomico = rand() % 4;
	if (bot->quadro == PARADO){
		switch (ataque_randomico){
		case 0:
			if (bot->pause_timer == 0 && !bot->acao->punch &&  !bot->acao->kick){
				joystick_kick(bot->control);
				bot->quadro = CHUTE;
			}
			break;
		case 1:
			if (bot->pause_timer == 0 && !bot->acao->kick && !bot->acao->punch){
				joystick_punch(bot->control);
				bot->quadro = SOCO;
			}
			break;
		case 2:
			if (bot->pause_timer == 0 && !bot->acao->kick && !bot->acao->punch){
				joystick_up(bot->control);
				joystick_punch(bot->control);
				bot->quadro = SOCO_PULANDO;
			}
			break;
		case 3:
			if (bot->pause_timer == 0 && !bot->acao->punch && !bot->acao->kick){
				joystick_up(bot->control);
				joystick_punch(bot->control);
				bot->quadro = CHUTE_PULANDO;
			}
			break;
		default:
			break;
		}
	}
	bot->quadro = PARADO;
}