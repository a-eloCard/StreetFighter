//gcc -Wall -Wextra -g aggressiveSquares.c Square.c Joystick.c Game.c Attack.c -o AS $(pkg-config --libs --cflags allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_ttf-5 allegro_image-5)
#include "Game.h"

#define ESTADO_INCIO 1
#define ROUND_TIMER 10

void draw_health_bar(square *player_1, square *player_2){
	unsigned int comp_barra = 350;
	unsigned char larg_barra = 20;
	unsigned int x_1 = 20 + comp_barra/2;
	unsigned int x_2 = X_SCREEN - x_1;
	unsigned char y = 50;
	float porcentagem_vida_1 = (float)player_1->hp/HP;
	float porcentagem_vida_2 = (float)player_2->hp/HP;
	int vida_barra_1 = comp_barra * porcentagem_vida_1;
	int vida_barra_2 = comp_barra * porcentagem_vida_2;

	al_draw_filled_rectangle(x_1 - comp_barra/2, y - larg_barra / 2 , x_1 + comp_barra/2, y + larg_barra / 2, al_map_rgb(255, 158, 179));
	al_draw_filled_rectangle(x_2 - comp_barra/2, y - larg_barra / 2 , x_2 + comp_barra/2, y + larg_barra / 2, al_map_rgb(255, 158, 179));

	al_draw_rectangle(x_1 - comp_barra/2 - 3, y - larg_barra / 2 -3, x_1 + comp_barra/2 + 3, y + larg_barra / 2+3, al_map_rgb(255, 255, 255),3);
	al_draw_rectangle(x_2 - comp_barra/2 - 3, y - larg_barra / 2 -3, x_2 + comp_barra/2 + 3, y + larg_barra / 2+3, al_map_rgb(255, 255, 255),3);

	al_draw_filled_rectangle(x_1 + comp_barra/2, y - larg_barra / 2, x_1 - comp_barra/2 + vida_barra_1, y + larg_barra / 2, al_map_rgb(144, 12, 63));
	al_draw_filled_rectangle(x_2 + comp_barra/2 - vida_barra_2, y - larg_barra / 2, x_2 - comp_barra/2, y + larg_barra / 2, al_map_rgb(144, 12, 63));
}

int main () {
    al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();	
	
	al_install_keyboard();																														

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);		
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();							
	ALLEGRO_FONT* font = al_load_ttf_font("./fonts/onesize.ttf", 35, 0);
	ALLEGRO_FONT* font_2 = al_load_ttf_font("./fonts/VCR.ttf", 17, 0);	
	ALLEGRO_FONT* font_3 = al_load_ttf_font("./fonts/8_bit.ttf", 17, 0);	
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);	
	ALLEGRO_BITMAP *fundo = al_load_bitmap("./backgrounds/fundo.png");				
	ALLEGRO_BITMAP *fim = al_load_bitmap("./backgrounds/gameover.png");
	ALLEGRO_BITMAP *tela_pause = al_load_bitmap("./backgrounds/pause.png");

	al_register_event_source(queue, al_get_keyboard_event_source());	
	al_register_event_source(queue, al_get_display_event_source(disp));	
	al_register_event_source(queue, al_get_timer_event_source(timer));																																	

	al_set_window_title(disp, "Street Fighter");

	const char *menu_items[] = {"Multiplayer", "Single player", "Sair"};
	const char *personagens[] = {"ken", "ryu"};
	int num_menu = 3;

	//obter dimensões da imagem
	int image_width = al_get_bitmap_width(fundo);
    int image_height = al_get_bitmap_height(fundo);

	bool done = false;
	ALLEGRO_EVENT event;
	int select = 0;
	bool single_player = 0;
	// Bloco de menu
	al_start_timer(timer);
	while (1) {
		al_wait_for_event(queue, &event);


		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_draw_scaled_bitmap(fundo, 0, 0, image_width, image_height, 0, 0, X_SCREEN, Y_SCREEN, 0);
			al_draw_text(font, al_map_rgb(255,255,255),  X_SCREEN/2, 340, ALLEGRO_ALIGN_CENTRE, "Selecione uma das opcoes:");
			int y;
			for (int i = 0; i < num_menu; i++) {
				y = 400 + (i * 40);			//calcular posição do item do menu
				if (i == select)
					al_draw_text(font, al_map_rgb(255,255,0),  X_SCREEN/2, y, ALLEGRO_ALIGN_CENTRE, menu_items[i]);			//desenhar o texto do item do menu
				else 
					al_draw_text(font, al_map_rgb(255,255,255),  X_SCREEN/2, y, ALLEGRO_ALIGN_CENTRE, menu_items[i]);
			}
    		al_flip_display();
		}

		else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
			if (event.keyboard.keycode == ALLEGRO_KEY_UP)
				select = (select - 1 + num_menu) % num_menu;
			else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
				select = (select + 1) % num_menu;
			
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
				if (select == 0){
					done = true;
					break;
				}
				else if (select == 1){
					single_player = 1;
					done = true;
					break;
				}
				else
					break;
			}
		}
		//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
	}

	ALLEGRO_BITMAP *ken = al_load_bitmap("./personagens/ken_menu.png");
	ALLEGRO_BITMAP *ryu = al_load_bitmap("./personagens/ryu_menu.png");

	movimento **draw_player_1 = cria_personagem(14);
	movimento **draw_player_2 = cria_personagem(14);

	unsigned int select_perso = 0;
	unsigned int select_player_1 = 0;
	unsigned int select_player_2 = 1;
	//Deixar aparecente o selecionado de cada um
	const char *selecionado_1 = "nada";
	unsigned int game_start = 0;
	while (done){
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			
			if (game_start == select_player_1)
				al_draw_text(font, al_map_rgb(255,255,255),  X_SCREEN/2, 100, ALLEGRO_ALIGN_CENTRE, "Jogador 1, selecione o personagem.");
			else 
				al_draw_text(font, al_map_rgb(255,255,255),  X_SCREEN/2, 100, ALLEGRO_ALIGN_CENTRE, "Jogador 2, selecione o personagem.");


			if (strcmp(selecionado_1, personagens[0]) == 0) {
				al_draw_rectangle(200 - 10, 200 - 10, 200 + 55*2.5 + 10, 200 + 93*2.5 + 10, al_map_rgb(255,0,0), 10);
			}
			else if (strcmp(selecionado_1, personagens[1]) == 0) {
				al_draw_rectangle(200 + (X_SCREEN-(55*2.5+200)-200) - 10, 200 - 10, 200 + (X_SCREEN-(55*2.5+200)-200) + 55*2.5 + 10, 200 + 93*2.5 + 10, al_map_rgb(255,0,0), 10);
			}


			al_draw_scaled_bitmap(ken, 0, 0, 55, 93, 200, 200, 55*2.5, 93*2.5, 0);
			al_draw_scaled_bitmap(ryu, 0, 0, 55, 93, X_SCREEN-(55*2.5+200), 200, 55*2.5, 93*2.5, 0);

			for (unsigned int i = 0; i < 2; i++){
				int x = 200 + i * (X_SCREEN-(55*2.5+200)-200);

				if (i == select_perso){
					if (game_start == select_player_1){
						al_draw_rectangle(x - 10, 200 - 10, x + 55*2.5 + 10, 200 + 93*2.5 + 10, al_map_rgb(255,0,0), 10);
					}
					else 
						al_draw_rectangle(x - 10, 200 - 10, x + 55*2.5 + 10, 200 + 93*2.5 + 10, al_map_rgb(0,0,255), 10);
				}
			}
    		al_flip_display();
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
			if (game_start == select_player_1){
				if (event.keyboard.keycode == ALLEGRO_KEY_A)
					select_perso = (select_perso - 1 + 2) % 2;
				else if (event.keyboard.keycode == ALLEGRO_KEY_D)
					select_perso = (select_perso + 1) % 2;
				else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
					inicializa_personagem(draw_player_1, 14, personagens[select_perso]);
					selecionado_1 = personagens[select_perso];
					game_start = select_player_2;
					select_perso = (select_perso + 1) % 2;
					if (single_player) {
						inicializa_personagem(draw_player_2, 14, personagens[select_perso]);
						done = true;
						break;
					}
				}
			}
			else{
				if (event.keyboard.keycode == ALLEGRO_KEY_LEFT){
					select_perso = (select_perso - 1 + 2) % 2;
					if (strcmp(selecionado_1, personagens[select_perso]) == 0){
						select_perso = (select_perso - 1 + 2) % 2;
					}
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
					select_perso = (select_perso + 1) % 2;
					if (strcmp(selecionado_1, personagens[select_perso]) == 0){
						select_perso = (select_perso - 1 + 2) % 2;
					}
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
					inicializa_personagem(draw_player_2, 14, personagens[select_perso]);
					done = true;
					break;
				}
			}
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = false;
			break;
		}
	}
	
	ALLEGRO_BITMAP *noite = al_load_bitmap("./backgrounds/noite.png");
	ALLEGRO_BITMAP *floresta = al_load_bitmap("./backgrounds/floresta.png");
	const char *cenario[] = {"noite", "floresta"};
	int selecionado_cenario = 0;

	unsigned int w_cenario = al_get_bitmap_width(noite);
    unsigned int h_cenario = al_get_bitmap_height(noite);

	while (done){
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_text(font, al_map_rgb(255,255,255),  X_SCREEN/2, 100, ALLEGRO_ALIGN_CENTRE, "Cenarios disponiveis: ");

			al_draw_scaled_bitmap(noite, 0, 0, w_cenario, h_cenario, 40, 200, w_cenario*0.75, h_cenario*0.75, 0);
			al_draw_scaled_bitmap(floresta, 0, 0, w_cenario, h_cenario, X_SCREEN-(w_cenario*0.75+40), 200, w_cenario*0.75, h_cenario*0.75, 0);

			for (int i = 0; i < 2; i++){
				int x = 40 + i * (X_SCREEN -(w_cenario*0.75 + 80));

				if (i == selecionado_cenario){
					al_draw_rectangle(x - 10, 200 - 10, x + w_cenario * 0.75 + 10, 200 + h_cenario * 0.75 + 10, al_map_rgb(254, 119, 118), 10);
				}
			}

			al_flip_display();
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN){
			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
				selecionado_cenario = (selecionado_cenario - 1 + 2) % 2;
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
				selecionado_cenario = (selecionado_cenario + 1) % 2;
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER){
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = false;
			break;
		}
	}

	// CRIAR OS JOGADORES
	square* player_1 = square_create(X_SIDE, Y_SIDE, 1, 200, CHAO - Y_SIDE/2, X_SCREEN, CHAO);	
	if (!player_1) return 1;														
	square* player_2 = square_create(X_SIDE, Y_SIDE, 0, X_SCREEN-200, CHAO - Y_SIDE/2, X_SCREEN, CHAO);
	if (!player_2) return 2;

	// Bloco com os personagens 
	unsigned char p1k = 0, p2k = 0;
	//controla frames
	float frame_cont_1 = 0.f, frame_cont_2 = 0.f;
	float current_frame_1 = 0.f, current_frame_2 = 0.f;

	unsigned short rounds = 0;

	char buffer_rounds[32];
	unsigned char game = ESTADO_INCIO;
	bool pause = false;
	int cont = 200;
	al_flush_event_queue(queue);

	bool final = 0;

	int p1_round_won = 0; // Player 1 ganhou a rodada atual
    int p2_round_won = 0;

	while (done) {
		al_wait_for_event(queue, &event);
		//VERIFICA QUEM GANHOU O JOGO
		if ((p1k >= 2 || p2k >= 2) && final){
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(fim, 0, 0, image_width, image_height, 0, 0, X_SCREEN, Y_SCREEN, 0);
			if (p1k >= 2 && p2k < 2){
				al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN - 300, 350, ALLEGRO_ALIGN_CENTER, "JOGADOR 1 VENCEU!");
				if (strcmp(selecionado_1, personagens[0]) == 0)
					al_draw_scaled_bitmap(ken, 0, 0, 55, 93, 100, 250, 55*2.5, 93*2.5, 0);
				else
					al_draw_scaled_bitmap(ryu, 0, 0, 55, 93, 100, 250, 55*2.5, 93*2.5, 0);
			}
			else if (p2k >= 2  && p1k < 2){
				al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN - 300, 350, ALLEGRO_ALIGN_CENTER, "JOGADOR 2 VENCEU!");
				if (strcmp(selecionado_1, personagens[0]) == 0)
					al_draw_scaled_bitmap(ryu, 0, 0, 55, 93, 100, 250, 55*2.5, 93*2.5, 0);
				else
					al_draw_scaled_bitmap(ken, 0, 0, 55, 93, 100, 250, 55*2.5, 93*2.5, 0);

			} 
			else if (p1k == 2 && p2k == 2) {
				al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTER, "EMPATOU!");
			}
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, 500, ALLEGRO_ALIGN_CENTRE, "PRESSIONE ESPACO PARA SAIR");
		
			al_flip_display();

			if ((event.type == 10) && (event.keyboard.keycode == ALLEGRO_KEY_SPACE)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (event.type == 42) break;
		}
		else {
			if (event.type == 30){
				if (!pause){
					update_position(player_1, player_2, single_player);	
					if (single_player){
						acao_single_player(player_2, player_1);
						//printf("x do ataque: %d, y do ataque: %d\n\n",player_2->punch->x, player_2->punch->y);
						//printf("x do personagem: %d, y do personagem: %d\n\n",player_2->x, player_2->y);
					}
					//printf("player 1 = %d, player 2 = %d\n", player_1->hp, player_2->hp);
					if (strcmp("noite", cenario[selecionado_cenario]) == 0){
						al_draw_scaled_bitmap(noite, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);

					}																																//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
					else if (strcmp("floresta", cenario[selecionado_cenario]) == 0){
						al_draw_scaled_bitmap(floresta, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);																																					//Substitui tudo que estava desenhado na tela por um fundo preto

					}	

					draw_health_bar(player_1, player_2);
					al_draw_textf(font_2, al_map_rgb(255,255,255), X_SCREEN/2, 40, ALLEGRO_ALIGN_CENTRE, "%dx%d", p1k, p2k);
					al_draw_text(font_3, al_map_rgb(255, 195, 0), 85, 70, ALLEGRO_ALIGN_CENTRE, "Player 1");
					al_draw_text(font_3, al_map_rgb(255, 195, 0), X_SCREEN-85, 70, ALLEGRO_ALIGN_CENTRE, "Player 2");


					draw_sprites(player_1);

					al_draw_scaled_bitmap(
						draw_player_1[player_1->quadro]->sprite,
						(int)frame_cont_1 * LARGURA_SPRITE, 0,
						LARGURA_SPRITE, draw_player_1[player_1->quadro]->altura,
						player_1->x - LARGURA_SPRITE, player_1->y - draw_player_1[player_1->quadro]->altura,
						LARGURA_SPRITE*2, draw_player_1[player_1->quadro]->altura*2, !player_1->face);

					current_frame_1 = (draw_player_1[player_1->quadro]->frames) / 40.0;
					
					frame_cont_1 += current_frame_1;
					if (frame_cont_1 >= (float)draw_player_1[player_1->quadro]->frames){
						frame_cont_1 = 0.0f;
					}
	
					draw_sprites(player_2);
					

					al_draw_scaled_bitmap(
						draw_player_2[player_2->quadro]->sprite,
						(int)frame_cont_2 * LARGURA_SPRITE, 0,
						LARGURA_SPRITE, draw_player_2[player_2->quadro]->altura,
						player_2->x - LARGURA_SPRITE, player_2->y - draw_player_2[player_2->quadro]->altura,
						LARGURA_SPRITE*2, draw_player_2[player_2->quadro]->altura*2, !player_2->face);

					current_frame_2 = (draw_player_2[player_2->quadro]->frames) / 40.0;
					
					frame_cont_2 += current_frame_2;

					if (frame_cont_2 >= (float)draw_player_2[player_2->quadro]->frames){
						frame_cont_2 = 0.0f;
					}

					if (game == ESTADO_INCIO){
						ALLEGRO_BITMAP *imagem1 = al_load_bitmap("./rounds/round1.png");
						al_draw_scaled_bitmap(imagem1, 0, 0, 360, 120,(X_SCREEN - 360/2)/2, (Y_SCREEN-120*2)/2, 360/2, 120/2, 0);
						al_flip_display();
						al_destroy_bitmap(imagem1);
						al_rest(1.2);			

						ALLEGRO_BITMAP *imagemF = al_load_bitmap("./rounds/fight.png");
						al_draw_scaled_bitmap(imagemF, 0, 0, 360, 120,(X_SCREEN - 360/2)/2, (Y_SCREEN-120)/2, 360/2, 120/2, 0);
						al_flip_display();
						al_rest(1.2);
						al_destroy_bitmap(imagemF);

						al_flush_event_queue(queue);

						game = 0;
					}

					if (player_1->acao->punch){
						player_1->timer--;
						//al_draw_filled_rectangle(player_1->punch->x - player_1->punch->side_x/2, player_1->punch->y - player_1->punch->side_y/2, player_1->punch->x + player_1->punch->side_x/2, player_1->punch->y + player_1->punch->side_y/2, al_map_rgb(255, 0, 0));
						if (!player_1->timer){
							player_1->acao->punch = 0;
							player_1->pause_timer = 20;
						}
					}
					else if (player_1->pause_timer > 0 )
						player_1->pause_timer--;

					if (player_2->acao->punch){
						player_2->timer--;
						//al_draw_filled_rectangle(player_2->punch->x - player_2->punch->side_x/2, player_2->punch->y - player_2->punch->side_y/2, player_2->punch->x + player_2->punch->side_x/2, player_2->punch->y + player_2->punch->side_y/2, al_map_rgb(0, 0, 255));
						if (!player_2->timer){
							player_2->acao->punch = 0;
							player_2->pause_timer = 25;
						}
					}
					else if (player_2->pause_timer > 0)
						player_2->pause_timer--;


					if (player_1->acao->kick){
						player_1->timer--;
						//al_draw_filled_rectangle(player_1->kick->x - player_1->kick->side_x/2, player_1->kick->y - player_1->kick->side_y/2, player_1->kick->x + player_1->kick->side_x/2, player_1->kick->y + player_1->kick->side_y/2, al_map_rgb(255, 0, 0));
						if (!player_1->timer){
							player_1->acao->kick = 0;
							player_1->pause_timer = 25;
						}
					}
					else if (player_1->pause_timer > 0)
						player_1->pause_timer--;
					
					if (player_2->acao->kick){
						//al_draw_filled_rectangle(player_2->kick->x - player_2->kick->side_x/2, player_2->kick->y - player_2->kick->side_y/2, player_2->kick->x + player_2->kick->side_x/2, player_2->kick->y + player_2->kick->side_y/2, al_map_rgb(0, 0, 255));
						player_2->timer--;
						if (!player_2->timer){
							player_2->acao->kick = 0;
							player_2->pause_timer = 25;
						}
					}
					else if (player_2->pause_timer > 0)
						player_2->pause_timer--;

					// conta a pontuação;
					if (player_1->hp <= 0 || player_2->hp <= 0){
						check_kill(player_1, player_2, &p1k, &p2k, &p1_round_won, &p2_round_won);

						rounds++;
							draw_sprites(player_1);
							draw_sprites(player_2);
						if(p1k < 2 && p2k < 2){
							if (p1_round_won && !p2_round_won){
								player_1->quadro = VITORIA1;
								player_2->quadro = PARADO;
							}
							else if(p2_round_won && !p1_round_won){
								player_1->quadro = PARADO;
								player_2->quadro = VITORIA1;
							}
							else if (p1_round_won && p2_round_won){
								player_1->quadro = PARADO;
								player_2->quadro = PARADO;
							}
							
							while(cont >= 0){
								if (strcmp("noite", cenario[selecionado_cenario]) == 0){
								al_draw_scaled_bitmap(noite, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);
								}																																//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
								else if (strcmp("floresta", cenario[selecionado_cenario]) == 0){
									al_draw_scaled_bitmap(floresta, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);																																					//Substitui tudo que estava desenhado na tela por um fundo preto
								}	

								draw_health_bar(player_1, player_2);
								al_draw_textf(font_2, al_map_rgb(255,255,255), X_SCREEN/2, 40, ALLEGRO_ALIGN_CENTRE, "%dx%d", p1k, p2k);
								al_draw_text(font_3, al_map_rgb(255, 195, 0), 85, 70, ALLEGRO_ALIGN_CENTRE, "Player 1");
								al_draw_text(font_3, al_map_rgb(255, 195, 0), X_SCREEN-85, 70, ALLEGRO_ALIGN_CENTRE, "Player 2");
								al_draw_scaled_bitmap(
									draw_player_1[player_1->quadro]->sprite,
									(int)frame_cont_1 * LARGURA_SPRITE, 0,
									LARGURA_SPRITE, draw_player_1[player_1->quadro]->altura,
									player_1->x - LARGURA_SPRITE, player_1->y - draw_player_1[player_1->quadro]->altura,
									LARGURA_SPRITE*2, draw_player_1[player_1->quadro]->altura*2, !player_1->face);
								current_frame_1 = (draw_player_1[player_1->quadro]->frames) / 100.0;
				
								frame_cont_1 += current_frame_1;
								if (frame_cont_1 >= (float)draw_player_1[player_1->quadro]->frames){
									frame_cont_1 = 0.0f;
								}

								al_draw_scaled_bitmap(
									draw_player_2[player_2->quadro]->sprite,
									(int)frame_cont_2 * LARGURA_SPRITE, 0,
									LARGURA_SPRITE, draw_player_2[player_2->quadro]->altura,
									player_2->x - LARGURA_SPRITE, player_2->y - draw_player_2[player_2->quadro]->altura,
									LARGURA_SPRITE*2, draw_player_2[player_2->quadro]->altura*2, !player_2->face);

								current_frame_2 = (draw_player_2[player_2->quadro]->frames) / 100.0;
				
								frame_cont_2 += current_frame_2;
								if (frame_cont_2 >= (float)draw_player_2[player_2->quadro]->frames){
									frame_cont_2 = 0.0f;
								}
								al_flip_display();
								cont--;
							}
							cont = 200;

							player_1->x = 200;
							player_2->x = X_SCREEN - 200;
							al_clear_to_color(al_map_rgb(0, 0, 0));
							if (p1_round_won && !p2_round_won){
								al_draw_textf(font_2, al_map_rgb(255, 0, 0), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTER, "JOGADOR 1 GANHOU O ROUND %d!", rounds);
							}
							else if(p2_round_won && !p1_round_won)
								al_draw_textf(font_2, al_map_rgb(0, 0, 255), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTER, "JOGADOR 2 GANHOU O ROUND %d!", rounds);
							else if (p1_round_won && p2_round_won)
								al_draw_textf(font_2, al_map_rgb(255,255,0), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTER, "EMPATE NO ROUND!");
							al_flip_display();
							al_rest(3);

							//APARECER VISUAL DA TELA DE JOGO
							if (strcmp("noite", cenario[selecionado_cenario]) == 0){
								al_draw_scaled_bitmap(noite, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);
							}																																
							else if (strcmp("floresta", cenario[selecionado_cenario]) == 0){
								al_draw_scaled_bitmap(floresta, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);																																					//Substitui tudo que estava desenhado na tela por um fundo preto
							}	

							player_1->quadro = PARADO;
							player_2->quadro = PARADO;

							draw_health_bar(player_1, player_2);
							al_draw_textf(font_2, al_map_rgb(255,255,255), X_SCREEN/2, 40, ALLEGRO_ALIGN_CENTRE, "%dx%d", p1k, p2k);
							al_draw_text(font_3, al_map_rgb(255, 195, 0), 85, 70, ALLEGRO_ALIGN_CENTRE, "Player 1");
							al_draw_text(font_3, al_map_rgb(255, 195, 0), X_SCREEN-85, 70, ALLEGRO_ALIGN_CENTRE, "Player 2");
							al_draw_scaled_bitmap(
								draw_player_1[player_1->quadro]->sprite,
								(int)frame_cont_1 * LARGURA_SPRITE, 0,
								LARGURA_SPRITE, draw_player_1[player_1->quadro]->altura,
								player_1->x - LARGURA_SPRITE, player_1->y - draw_player_1[player_1->quadro]->altura,
								LARGURA_SPRITE*2, draw_player_1[player_1->quadro]->altura*2, !player_1->face);
							current_frame_1 = (draw_player_1[player_1->quadro]->frames) / 100.0;
			
							frame_cont_1 += current_frame_1;
							if (frame_cont_1 >= (float)draw_player_1[player_1->quadro]->frames){
								frame_cont_1 = 0.0f;
							}

							al_draw_scaled_bitmap(
								draw_player_2[player_2->quadro]->sprite,
								(int)frame_cont_2 * LARGURA_SPRITE, 0,
								LARGURA_SPRITE, draw_player_2[player_2->quadro]->altura,
								player_2->x - LARGURA_SPRITE, player_2->y - draw_player_2[player_2->quadro]->altura,
								LARGURA_SPRITE*2, draw_player_2[player_2->quadro]->altura*2, !player_2->face);

							current_frame_2 = (draw_player_2[player_2->quadro]->frames) / 100.0;
			
							frame_cont_2 += current_frame_2;
							if (frame_cont_2 >= (float)draw_player_2[player_2->quadro]->frames)
								frame_cont_2 = 0.0f;
							
							sprintf(buffer_rounds, "./rounds/round%d.png", rounds+1);
							ALLEGRO_BITMAP *imagem1 = al_load_bitmap(buffer_rounds);
							al_draw_scaled_bitmap(imagem1, 0, 0, 360, 120,(X_SCREEN - 360/2)/2, (Y_SCREEN-120*2)/2, 360/2, 120/2, 0);
							al_flip_display();
							al_rest(2.5);
							al_destroy_bitmap(imagem1);

							ALLEGRO_BITMAP *imagemF = al_load_bitmap("./rounds/fight.png");
							al_draw_scaled_bitmap(imagemF, 0, 0, 360, 120,(X_SCREEN - 360/2)/2, (Y_SCREEN-120)/2, 360/2, 120/2, 0);
							al_flip_display();
							al_rest(1.5);
							al_destroy_bitmap(imagemF);

							p1_round_won = 0;
							p2_round_won = 0;

							reset_joystick(player_1->control);
							reset_joystick(player_2->control);

							al_flush_event_queue(queue);
						}
						// PARA O GANHADOR FINAL
						else{
							draw_sprites(player_1);
							draw_sprites(player_2);
							if (p1k == 2 && p2k < 2 && p1_round_won && !p2_round_won){
								player_1->quadro = VITORIA2;
								player_2->quadro = PARADO;
							}
							else if(p2k == 2 && p1k < 2 && p2_round_won && !p1_round_won){
								player_1->quadro = PARADO;
								player_2->quadro = VITORIA2;
							}
							else if (p1k == 2 && p2k == 2 && p1_round_won && p2_round_won){
								player_1->quadro = PARADO;
								player_2->quadro = PARADO;
							}
							while(cont >= 0){
								if (strcmp("noite", cenario[selecionado_cenario]) == 0){
								al_draw_scaled_bitmap(noite, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);
								}																																//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
								else if (strcmp("floresta", cenario[selecionado_cenario]) == 0){
									al_draw_scaled_bitmap(floresta, 0, 0, w_cenario, h_cenario, 0, 0, X_SCREEN, Y_SCREEN, 0);																																					//Substitui tudo que estava desenhado na tela por um fundo preto
								}	

								draw_health_bar(player_1, player_2);
								al_draw_textf(font_2, al_map_rgb(255,255,255), X_SCREEN/2, 40, ALLEGRO_ALIGN_CENTRE, "%dx%d", p1k, p2k);
								al_draw_text(font_3, al_map_rgb(255, 195, 0), 85, 70, ALLEGRO_ALIGN_CENTRE, "Player 1");
								al_draw_text(font_3, al_map_rgb(255, 195, 0), X_SCREEN-85, 70, ALLEGRO_ALIGN_CENTRE, "Player 2");
								al_draw_scaled_bitmap(
									draw_player_1[player_1->quadro]->sprite,
									(int)frame_cont_1 * LARGURA_SPRITE, 0,
									LARGURA_SPRITE, draw_player_1[player_1->quadro]->altura,
									player_1->x - LARGURA_SPRITE, player_1->y - draw_player_1[player_1->quadro]->altura,
									LARGURA_SPRITE*2, draw_player_1[player_1->quadro]->altura*2, !player_1->face);
								current_frame_1 = (draw_player_1[player_1->quadro]->frames) / 100.0;
				
								frame_cont_1 += current_frame_1;
								if (frame_cont_1 >= (float)draw_player_1[player_1->quadro]->frames){
									frame_cont_1 = 0.0f;
								}

								al_draw_scaled_bitmap(
									draw_player_2[player_2->quadro]->sprite,
									(int)frame_cont_2 * LARGURA_SPRITE, 0,
									LARGURA_SPRITE, draw_player_2[player_2->quadro]->altura,
									player_2->x - LARGURA_SPRITE, player_2->y - draw_player_2[player_2->quadro]->altura,
									LARGURA_SPRITE*2, draw_player_2[player_2->quadro]->altura*2, !player_2->face);

								current_frame_2 = (draw_player_2[player_2->quadro]->frames) / 100.0;
				
								frame_cont_2 += current_frame_2;
								if (frame_cont_2 >= (float)draw_player_2[player_2->quadro]->frames){
									frame_cont_2 = 0.0f;
								}
								al_flip_display();
								cont--;
							}
							cont = 200;
							p1_round_won = 0;
							p2_round_won = 0;
						}
						final = 1;
					}	
					al_flip_display();																																								//Insere as modificações realizadas nos buffers de tela
				}
				else {
					al_draw_scaled_bitmap(tela_pause, 0, 0, image_width, image_height, 0, 0, X_SCREEN, Y_SCREEN, 0);
					al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 300, ALLEGRO_ALIGN_CENTRE, "PAUSE");
					al_flip_display();
				}
			}
			else if ((event.type == 10) || (event.type == 12)){		
				if (single_player)
					movimentacao_single(event, player_1);
				else 
					movimentacao(event, player_1, player_2);
				if (event.keyboard.keycode == ALLEGRO_KEY_P && (event.type == 10)){
					pause = !pause;
				}
			}
			else if (event.type == 42) break;																																								//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
		}
	}
	
	al_destroy_bitmap(fundo);
	al_destroy_bitmap(fim);
	al_destroy_bitmap(tela_pause);
	al_destroy_bitmap(ken);
	al_destroy_bitmap(ryu);
	al_destroy_bitmap(noite);
	al_destroy_bitmap(floresta);
	al_destroy_font(font);
	al_destroy_font(font_2);
	al_destroy_font(font_3);		
	al_destroy_display(disp);	
	al_destroy_timer(timer);	
	al_destroy_event_queue(queue);
	square_destroy(player_1);																								
	square_destroy(player_2);
	liberar_personagens(draw_player_1, 14);
	liberar_personagens(draw_player_2, 14);

	return 0;					
}
