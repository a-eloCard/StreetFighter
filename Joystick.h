#ifndef __JOYSTICK__ 																												//Guardas de inclusão (!)
#define __JOYSTICK__																												//Guardas de inclusão (!)

typedef struct {																													//Definição da estrutura de um controle (!)
	unsigned char right;																											//Botão de movimentação à direta (!)
	unsigned char left;																												//Botão de movimentação à esquerda (!)
	unsigned char up;																												//Botão de movimentação para cima (!)
	unsigned char down;	
	unsigned char punch;	
	unsigned char kick;	
	unsigned char parry;																									//Botão de movimentação para baixo (!)
} joystick;																															//Definição do nome da estrutura (!)

joystick* joystick_create();																										//Protótipo da função de criação de um controle (!)
void joystick_destroy(joystick *element);																							//Protótipo da função de destruição de um controle (!)
void joystick_right(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à direita (!)
void joystick_left(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à esquerda (!)
void joystick_up(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação para cima (!)
void joystick_down(joystick *element);		
void joystick_punch(joystick *element);	
void joystick_kick(joystick *element);																							//Protótipo da função de ativação/desativação do botão de soco																						//Protótipo da função de ativação/desativação do botão de movimentação para baixo (!)
void joystick_parry(joystick *element);	
void reset_joystick(joystick *element);

#endif																																//Guardas de inclusão (!)