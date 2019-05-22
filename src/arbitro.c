/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "screen.h"
#include "i386.h"

#define COLOR_V C_BG_BLACK
#define COLOR_1 C_FG_WHITE + C_BG_RED
#define COLOR_2 C_FG_WHITE + C_BG_CYAN
#define COLOR_3 C_FG_WHITE + C_BG_GREEN
#define COLOR_4 C_FG_WHITE + C_BG_BLUE
#define COLOR_A C_FG_WHITE + C_BG_MAGENTA

#define TABLERO_POS_FIL 3
#define TABLERO_POS_COL 3

void dibujar_interfaz();
void imprimir_tablero();
void imprimir_puntaje(int* puntaje);
void imprimir_ganador();
int  juego_terminado(int* puntaje);
void actualizar_pantalla(int* puntaje);
void calcular_puntajes(int* puntaje);

void task() 
{
	dibujar_interfaz();
	syscall_iniciar();
	int puntaje[5]; //puntaje[4] es para la celdas vacías
	while(1) {
		calcular_puntajes(puntaje);
		actualizar_pantalla(puntaje);
		if(juego_terminado(puntaje)) {
			imprimir_ganador(puntaje);
			syscall_terminar();
			//Salgo del while y me quedo ciclando en el otro hasta pasar al sched
			break;
		}
	}

	while(1) {};
}

void dibujar_interfaz() 
{
	rect(C_BG_BLACK,      1,  1, 25, 80);
	rect(C_BG_LIGHT_GREY, 2,  1, 19, 80);
	rect(C_BG_BROWN,      20, 1, 24, 80);
	rect(C_BG_MAGENTA, 8,  49, 14, 65);
	aprintf(8, 49, C_FG_GREEN | C_BG_BROWN, "Points table");
	aprintf(20, 1, COLOR_1, "|1");
	aprintf(21, 1, COLOR_2, "|2");
	aprintf(22, 1, COLOR_3, "|3");
	aprintf(23, 1, COLOR_4, "|4");
	aprintf(24, 1, COLOR_A, "|A");
	aprintf(1, 20, C_FG_GREEN, "Kernel");
}

void calcular_puntajes(int* puntaje) 
{
	int fil, col;

	unsigned char (*tablero)[TABLERO_COLS] = (unsigned char (*)[TABLERO_COLS]) (TABLERO_ADDR);
	puntaje[0] = 0;
	puntaje[1] = 0;
	puntaje[2] = 0;
	puntaje[3] = 0;
	puntaje[4] = 0;

	for(fil = 0; fil < TABLERO_FILS; fil++) {
		for(col = 0; col < TABLERO_COLS; col++) {
			switch(tablero[fil][col]) {
				case JUG_1:
					puntaje[0]++;
					break;
				case JUG_2:
					puntaje[1]++;
					break;
				case JUG_3:
					puntaje[2]++;
					break;
				case JUG_4:
					puntaje[3]++;
					break;
				default:
					puntaje[4]++;
					break;					
			}
		}
	}
}

void actualizar_pantalla(int* puntaje) {
	imprimir_puntaje(puntaje);
	imprimir_tablero();
}

int juego_terminado(int* puntaje) {
	if(puntaje[4] == 0) {
		return TRUE;
	}
	return FALSE;
}

void imprimir_ganador(int* puntaje) {
	int i,j=0;
	int puntajeMax = puntaje[0];
	for(i=1;i<4;i++) {
		if(puntajeMax < puntaje[i]) {
			puntajeMax = puntaje[i];
			j = i;
		}
	}
	aprintf(3, 50, COLOR_1, "Juego Terminado!");
	aprintf(4, 50, COLOR_1, "El jugador %u es el ganador!", j+1);
}

void imprimir_puntaje(int* puntaje) {
	aprintf(10, 50, COLOR_1, "Jugador 1: %d", puntaje[0]);
	aprintf(11, 50, COLOR_2, "Jugador 2: %d", puntaje[1]);
	aprintf(12, 50, COLOR_3, "Jugador 3: %d", puntaje[2]);
	aprintf(13, 50, COLOR_4, "Jugador 4: %d", puntaje[3]);
}

void imprimir_tablero() {
	int fil, col;

	unsigned char (*tablero)[TABLERO_COLS] = (unsigned char (*)[TABLERO_COLS]) (TABLERO_ADDR);

	for(fil = 0; fil < TABLERO_FILS; fil++) {
		for(col = 0; col < TABLERO_COLS; col++) {
			switch(tablero[fil][col]) {
				case JUG_1:
					rect(COLOR_1, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_2:
					rect(COLOR_2, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_3:
					rect(COLOR_3, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case JUG_4:
					rect(COLOR_4, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);
					break;
				case TABLERO_CELDA_VACIA:
					rect(COLOR_V, TABLERO_POS_FIL + fil, TABLERO_POS_COL + col,
								  TABLERO_POS_FIL + fil, TABLERO_POS_COL + col);				
					break;					
			}
		}
	}
}
