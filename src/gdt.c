/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
Global descritors table
*/
#include "defines.h"
#include "gdt.h"
#include "tss.h"

gdt_entry gdt[GDT_COUNT] = {
	/* Descriptor nulo*/
	/* Offset = 0x00 */
	[GDT_IDX_NULL_DESC] = (gdt_entry) {
		(unsigned short)	0x0000,			/* limit[0:15]  */
		(unsigned short)	0x0000,			/* base[0:15]   */
		(unsigned char)		0x00,			/* base[23:16]  */
		(unsigned char)		0x00,			/* type         */
		(unsigned char)		0x00,			/* s            */
		(unsigned char)		0x00,			/* dpl          */
		(unsigned char)		0x00,			/* p            */
		(unsigned char)		0x00,			/* limit[16:19] */
		(unsigned char)		0x00,			/* avl          */
		(unsigned char)		0x00,			/* l            */
		(unsigned char)		0x00,			/* db           */
		(unsigned char)		0x00,			/* g            */
		(unsigned char)		0x00,			/* base[31:24]  */
	},

	//Code 1, privilegio 0
	[1] = {
		//Limit: máximo offset que puede tener un byte en un segmento
		.limit_0_15  = 0xFFFF,
		//Base: dirección a partir de la cuál se tiene el segmento
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 10, //101
		//System: s = 0 => recurso del kernel, s=1 => codigo o datos
		.s           = 1,
		//Descriptor de privilegios.
		.dpl         = 0,
		//Present: p = 1 => segmento presente en la ram
		.p           = 1,
		.limit_16_19 = 0x7,
		//Available: bit sin uso
		.avl         = 0,
		//L siempre 0, => 32 bits, L=1 y db = 0 => 64 bits de segmento
		.l           = 0,
		//Default Big: configura el tamaño del segmento, si db = 1, 32 bits
		//si db = 0, 16 bits
		.db          = 1,
		//Granularidad: establece la unidad de medida de "límite",
		//Si g = 1, el límite es multiplicado por 4096
		//Si g = 0, el límite es tal cuál está
		.g           = 1,
		.base_31_24  = 0,
	},
	//Code 2, privilegio 2
	[2] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 10,
		.s           = 1,
		.dpl         = 2,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},
	//Code 3, privilegio 3
	[3] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 10,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},
	//Data 1, privilegio 0
	[4] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2, //0010
		.s           = 1,
		.dpl         = 0,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},
	//Data 2, privilegio 2
	[5] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2,
		.s           = 1,
		.dpl         = 2,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},
	//Data 3, privilegio 3
	[6] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = 2,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	},
	//privilegio 0
	[7] = {
		.limit_0_15  = 0x8000,
		.base_0_15   = 0x8000,
		.base_23_16  = 0xB,
		.type        = 2,
		.s           = 1,
		.dpl         = 3,
		.p           = 1,
		.limit_16_19 = 0,
		.avl         = 0,
		.l           = 0,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	}
};

gdt_descriptor GDT_DESC = {
	sizeof(gdt) - 1,
	(unsigned int) &gdt
};
