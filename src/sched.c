/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/
#include "defines.h"
#include "screen.h"
#include "sched.h"


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

unsigned short sched_proximo_indice() {
	unsigned short 	result;
	int 			cant = 0;

	if (!arbitro)
  {
		result 	= arb_pos;
		arbitro = 1;
	}else
  {
		while (tareas[t_index] == 0 && cant < 4) 
    {
			++t_index;
			++cant;
		}
		if(cant == 4) 
    {
			return arb_pos;
		}

		result 	= tareas[t_index];
		arbitro = 1;
		++t_index;
	} 

	return result;
}

void sched_remover_tarea(unsigned int process_id)
{
  tareas[process_id] = 0;
}

