/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

typedef struct str_pd_entry {
    unsigned char present:1;
    unsigned char read_write:1;
    unsigned char user_supervisor:1;
    unsigned char page_level_write_through:1;
    unsigned char page_level_cache_disable:1;
    unsigned char accessed:1;
    unsigned char ignored_6:1;
    unsigned char page_size:1;
    unsigned char global:1;
    unsigned char ignored_9_11:3;
    unsigned int  address:20;
} __attribute__((__packed__, aligned (4))) pd_entry;

typedef struct str_pt_entry {
    unsigned char present:1;
    unsigned char read_write:1;
    unsigned char user_supervisor:1;
    unsigned char page_level_write_through:1;
    unsigned char page_level_cache_disable:1;
    unsigned char accessed:1;
    unsigned char dirty:1;
    unsigned char page_table_attribute_index:1;
    unsigned char global:1;
    unsigned char ignored_9_11:3;
    unsigned int  address:20;
} __attribute__((__packed__, aligned (4))) pt_entry;

pd_entry* tasks_directory[5];
unsigned int last_real_dir;
unsigned short page_counter[5];

char set_memory(unsigned int direccion);

// inicializa el directorio de páginas del kernel
void mmu_init_page_and_table_directory();

// inicializa el mmu
void mmu_inicializar();


#endif	/* !__MMU_H__ */
