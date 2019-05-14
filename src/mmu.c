/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "defines.h"
#include "mmu.h"
#include "i386.h"

void mmu_init_page_and_table_directory() {

	pd_entry* page_directory = (pd_entry*) KERNEL_PAGE_DIR;
	int i;
	for (i = 0; i < 1024; i++) {
		*((unsigned int*) &page_directory[i]) = 0;
	}
	page_directory[0].user_supervisor = 0;
	page_directory[0].address = KERNEL_PAGE_TABLE >> 12;
	page_directory[0].read_write = 1;
	page_directory[0].present = 1;

	pt_entry* page_table = (pt_entry*) KERNEL_PAGE_TABLE;
	int i;
	for (i = 0; i < 1024; i++) 
  {
		*((unsigned int*) &(page_table[i])) = 0;
	}

	 for (i = 0; i < 1024; i++) 
   {
	 	if(i < 0x164) {
	 		page_table[i].user_supervisor = 0;
	 		page_table[i].address = i;
	 		page_table[i].read_write = 1;
	 		page_table[i].present = 1;
	 	}
	 }
}

void mmu_mappear_pagina(unsigned int virtual,
                       unsigned int fisica,
                       pd_entry* page_directory,
                       unsigned char user_supervisor,
                       unsigned char read_write) {
    
    // Dirección virtual: 
    //
    // | Directorio   | Table        | Offset         |
    // | XXXX XXXX XX | XX XXXX XXXX | XXXX XXXX XXXX |
    // | 31        22 | 21        12 | 11           0 |

    unsigned int directory = (virtual >> 22) & 0x3FF;
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    pt_entry* page_table = (pt_entry*) (page_directory[directory].address << 12);

    page_table[table].user_supervisor = user_supervisor;
    page_table[table].address = fisica >> 12;
    page_table[table].read_write = read_write;
    page_table[table].present = 1;
}

void mmu_inicializar_tarea_jugador
(
        pd_entry* page_directory,
        pt_entry* page_table,
        unsigned int code_address,
        unsigned int stack_address) {
    int i;

    // Inicializo el directorio con todas las entradas vacías
    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &page_directory[i]) = 0;
    }

    // Asigno una única entrada en el directorio apuntando a la tabla
    page_directory[0].user_supervisor = 1;
    page_directory[0].address = ((unsigned int) page_table) >> 12;
    page_directory[0].read_write = 1;
    page_directory[0].present = 1;

    // Inicializo la tabla con todas las entradas vacías
    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &(page_table[i])) = 0;
    }

    // Mappeo la memoria
    mmu_mappear_pagina(TASK_CODE, code_address, page_directory, 1, 1);
    mmu_mappear_pagina(TASK_STACK, stack_address, page_directory, 1, 1);
    mmu_mappear_pagina(TABLERO_ADDR, TABLERO_ADDR_PA, page_directory, 1, 0);
}

void mmu_inicializar_tarea_arbitro()
{
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_5_PAGE_DIR,
        (pt_entry*) TASK_5_PAGE_TABLE,
        TASK_5_CODE_PA,
        TASK_5_STACK_PA);

    mmu_mappear_pagina(VIDEO_ADDR, VIDEO_ADDR, (pd_entry*) TASK_5_PAGE_DIR, 1, 1);

}


void mmu_inicializar() 
{
    // Jugador 1
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_1_PAGE_DIR,
        (pt_entry*) TASK_1_PAGE_TABLE,
        TASK_1_CODE_PA,
        TASK_1_STACK_PA);

    // Jugador 2
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_2_PAGE_DIR,
        (pt_entry*) TASK_2_PAGE_TABLE,
        TASK_2_CODE_PA,
        TASK_2_STACK_PA);

    // Jugador 3
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_3_PAGE_DIR,
        (pt_entry*) TASK_3_PAGE_TABLE,
        TASK_3_CODE_PA,
        TASK_3_STACK_PA);

    // Jugador 4
    mmu_inicializar_tarea_jugador(
        (pd_entry*) TASK_4_PAGE_DIR,
        (pt_entry*) TASK_4_PAGE_TABLE,
        TASK_4_CODE_PA,
        TASK_4_STACK_PA);

    // Árbitro
    mmu_inicializar_tarea_arbitro();
}

