/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/
#include "defines.h"
#include "mmu.h"
#include "i386.h"


void mmu_pagemap(unsigned int virtual,
                       unsigned int fisica,
                       pd_entry* page_directory,
                       unsigned char user_supervisor,
                       unsigned char read_write) {
    
    unsigned int directory = (virtual >> 22) & 0x3FF;
    unsigned int table     = (virtual & 0x003FF000) >> 12;

    pt_entry* page_table = (pt_entry*) (page_directory[directory].address << 12);

    page_table[table].user_supervisor = user_supervisor;
    page_table[table].address = fisica >> 12;
    page_table[table].read_write = read_write;
    page_table[table].present = 1;
}

char set_memory(unsigned int direccion) 
{
    unsigned short tarea = current_player()-10; 
    if((direccion >= 0x003D0000) && (direccion < 0x003F0000) && (page_counter[tarea] <= 5)) 
    {
        page_counter[tarea]++;
        mmu_pagemap(direccion, last_real_dir, (pd_entry*)tasks_directory[tarea], 0, 1);
        last_real_dir += 4096; 
        return 1;
    }
    else {
        return 0;
    }
} 

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
   mmu_pagemap(TASK_CODE, 0x00010000, (pd_entry*)KERNEL_PAGE_DIR, 0, 1);
}

void mmu_init_player_task
(
        pd_entry* page_directory,
        pt_entry* page_table,
        unsigned int code_address,
        unsigned int stack_address) {
    int i;

    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &page_directory[i]) = 0;
    }

    page_directory[0].user_supervisor = 1;
    page_directory[0].address = ((unsigned int) page_table) >> 12;
    page_directory[0].read_write = 1;
    page_directory[0].present = 1;

    for (i = 0; i < 1024; i++) {
        *((unsigned int*) &(page_table[i])) = 0;
    }

    for (i = 0; i < 0x164; i++) {
        page_table[i].user_supervisor = 0;
        page_table[i].address = i;
        page_table[i].read_write = 1;
        page_table[i].present = 1;
    }

    mmu_pagemap(TASK_CODE, code_address, page_directory, 1, 1);
    mmu_pagemap(TASK_STACK, stack_address, page_directory, 1, 1);
    mmu_pagemap(TABLERO_ADDR, TABLERO_ADDR_PA, page_directory, 1, 0);
}

void mmu_inicializar_tarea_arbitro()
{
    mmu_init_player_task(
        (pd_entry*) TASK_5_PAGE_DIR,
        (pt_entry*) TASK_5_PAGE_TABLE,
        TASK_5_CODE_PA,
        TASK_5_STACK_PA);

    mmu_pagemap(VIDEO_ADDR, VIDEO_ADDR, (pd_entry*) TASK_5_PAGE_DIR, 1, 1);

}


void mmu_inicializar() 
{
    mmu_init_player_task(
        (pd_entry*) TASK_1_PAGE_DIR,
        (pt_entry*) TASK_1_PAGE_TABLE,
        TASK_1_CODE_PA,
        TASK_1_STACK_PA);

    mmu_init_player_task(
        (pd_entry*) TASK_2_PAGE_DIR,
        (pt_entry*) TASK_2_PAGE_TABLE,
        TASK_2_CODE_PA,
        TASK_2_STACK_PA);

    mmu_init_player_task(
        (pd_entry*) TASK_3_PAGE_DIR,
        (pt_entry*) TASK_3_PAGE_TABLE,
        TASK_3_CODE_PA,
        TASK_3_STACK_PA);

    mmu_init_player_task(
        (pd_entry*) TASK_4_PAGE_DIR,
        (pt_entry*) TASK_4_PAGE_TABLE,
        TASK_4_CODE_PA,
        TASK_4_STACK_PA);

    // Árbitro
    mmu_inicializar_tarea_arbitro();
}

