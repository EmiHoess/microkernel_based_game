; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start


;; GDT
extern GDT_DESC

;; IDT
extern IDT_DESC
extern idt_inicializar

;; PIC
extern disable_pic
extern reset_pic
extern enable_pic

;;MMU
extern mmu_init_page_and_table_directory

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;;
iniciando_mr_msg db		'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ	$ - iniciando_mr_msg

iniciando_mp_msg db		'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ	$ - iniciando_mp_msg

welcome_msg db		'Hello!'
welcome_msg_len equ		$ - welcome_msg

;;
;; Seccion de código.
;;

;; Punto de entrada del kernel.
BITS 16
start:
	; Deshabilitar interrupciones
	cli

	; Imprimir mensaje de bienvenida
	imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

	xchg bx, bx

	; enable A20 line
	call habilitar_A20

	; load GDT

	lgdt [GDT_DESC]
	; set PE bit from CR0
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	; protected mode on
	jmp 0x8:protected_mode
	BITS 32
protected_mode:
	; sort segments
	mov ax, 32
	mov ss, ax 
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov ax, 56
	mov fs, ax
	; set stack
	mov EBP, 0x20000
	mov ESP, 0x20000
	
	mov ecx, 0x7FFF
	set_zeros:
		mov byte [fs:ecx], 0
		loop set_zeros
	imprimir_texto_mp welcome_msg, welcome_msg_len, 0x06, 0, 0
	; inicializar el manejador de memoria

	; inicializar el directorio de paginas
	call mmu_init_page_and_table_directory
	; inicializar memoria de tareas
	call mmu_inicializar
	
	; habilitar paginacion
	mov eax, 0x00021000 ; 
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000000 ; 
	mov cr0, eax

	imprimir_texto_mp bienvenida_msg, bienvenida_len, 0x06, 0, 0

	; inicializar tarea idle

	; inicializar todas las tsss

	; inicializar entradas de la gdt de tss

	; inicializar el scheduler

	; inicializar la IDT
	call idt_inicializar
	lidt [IDT_DESC]
	
	; configurar controlador de interrupciones
	call disable_pic
	call reset_pic
	call enable_pic
	sti
	; cargo la primer tarea null

	; aca salto a la primer tarea

	; Ciclar infinitamente (por si algo sale mal)
	jmp $

%include "a20.asm"
