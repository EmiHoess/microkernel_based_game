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

void mmu_inicializar() {
}

