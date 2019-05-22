/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "screen.h"
#include "sched.h"

char		   restart	= 1;
char		   pause			= 0;
char 		   index			= 0;
char		   quantum			= 2;
char		   end		= 0;

#define COLOR_V C_BG_BLACK
#define COLOR_1 C_FG_WHITE + C_BG_RED
#define COLOR_2 C_FG_WHITE + C_BG_CYAN
#define COLOR_3 C_FG_WHITE + C_BG_GREEN
#define COLOR_4 C_FG_WHITE + C_BG_BLUE
#define COLOR_A C_FG_WHITE + C_BG_MAGENTA

unsigned char clock[4] = {'|', '/', '-', '\\'};
unsigned int clock_values[5] = {0,0,0,0,0};

unsigned short tareas[CANT_TAREAS];
unsigned short arb_pos = 112;
unsigned short t_index = 0;
char arbitro = 0;


void init_sched() 
{
  tareas[0] = 80;
  tareas[1] = 88;
  tareas[2] = 96;
  tareas[3] = 104;
}

void task_clock() {
	char buf[2];
	buf[1] = '\0';
	unsigned short tarea = tarea_actual();
	if(tarea == 0) return;
	clock_values[tarea-1]++;
	if(clock_values[tarea-1] == 4) clock_values[tarea-1] = 0;
	buf[0] = clock[clock_values[tarea-1]];
	switch(tarea) {
		case 1:
			aprintf(20, 1, COLOR_1, buf);
			break;
		case 2:
			aprintf(21, 1, COLOR_2, buf);
			break;
		case 3:
			aprintf(22, 1, COLOR_3, buf);
			break;
		case 4:
			aprintf(23, 1, COLOR_4, buf);
			break;
		case 5:
			aprintf(24, 1, COLOR_A, buf);
			break;
	}
}

void game_tick() 
{
	if(end == 1) return;
	if (quantum > 0)
	{
		quantum--;
		return;
	}
	if(quantum == 0) 
	{
		quantum = 2;
		if(pause == 0 && restart == 0) 
		{ 
			pause 		= 1;
			jmp_to_task(72); 	
		} else if(restart == 1) 
		{  
			unsigned short proxTarea = sched_proximo_indice();
			pause = 0;
			breakpoint();
			if(proxTarea != 0)
			{
				
				jmp_to_task(proxTarea); 
			} else 
			{
				end = 1;
				jmp_to_task(72); 	
			} 
		}
	}
	task_clock();
}


unsigned short current_player()
{
	unsigned short ret_task = 0;
	int i;
	for(i = 10; i <= 13; i++) 
	{
		unsigned short busy = (gdt[i].type & 0x0002);
		if(busy == 2) ret_task = i - 9;
	}
	return ret_task;
}


unsigned short sched_proximo_indice() {
	unsigned short 	result;
	unsigned short	cant 	= 0;

	if (arbitro == FALSE) 
	{
		result 	= arb_pos;
		arbitro = TRUE;
	} else 
	{
		for (;tareas[t_index] == 0 && cant < 4;++posicion,++cant) {}
		

		if(cant == 4) return 0;
		result 	= tareas[t_index];
		arbitro = FALSE;
		posicion = (posicion+1)%4;
	} 
	return result;
}

void sched_remove_task(unsigned int process_id)
{
	tareas[process_id] = 0;
	quantum = 0;
}

void spend_turn() {
	quantum = 0;
	game_tick();
}

