/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "defines.h"
#include "game.h"
#include "syscall.h"

void task() {
	/* Task 1: Tarea jugador 1 */
	while(1) 
	{
        syscall_duplicar(1, 1);
        syscall_duplicar(2, 2);
        syscall_duplicar(3, 3);
		
        syscall_duplicar(10, 10);

        syscall_duplicar(4, 4);
        syscall_duplicar(5, 5);
        syscall_duplicar(6, 6);        
	};
}