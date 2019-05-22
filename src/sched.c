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
		printf(2, 50, "Pos 1: %d", posicion);
		for (;tareas[t_index] == 0 && cant < 4;++posicion,++cant) {}
		printf(3, 50, "Pos = %u, # = %u", posicion, cant);


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


