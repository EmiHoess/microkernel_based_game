/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
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

	[1] = {
		.limit_0_15  = 0xFFFF,
		.base_0_15   = 0,
		.base_23_16  = 0,
		.type        = ,
		.s           = ,
		.dpl         = ,
		.p           = 1,
		.limit_16_19 = 0x7,
		.avl         = 0,
		.l           = ,
		.db          = 1,
		.g           = 1,
		.base_31_24  = 0,
	}
};

gdt_descriptor GDT_DESC = {
	sizeof(gdt) - 1,
	(unsigned int) &gdt
};
