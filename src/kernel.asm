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
extern resetear_pic
extern habilitar_pic


;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;;
iniciando_mr_msg db		'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ	$ - iniciando_mr_msg

iniciando_mp_msg db		'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ	$ - iniciando_mp_msg

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
	xchg bx, bx
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	; protected mode on
	jmp 0x8:modo_protegido
	BITS 32
	modo_protegido:
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
	; pintar pantalla, todos los colores, que bonito!

	; inicializar el manejador de memoria

	; inicializar el directorio de paginas

	; inicializar memoria de tareas

	; habilitar paginacion

	; inicializar tarea idle

	; inicializar todas las tsss

	; inicializar entradas de la gdt de tss

	; inicializar el scheduler

	; inicializar la IDT
	lidt [IDT_DESC]
	; configurar controlador de interrupciones

	; cargo la primer tarea null

	; aca salto a la primer tarea

	; Ciclar infinitamente (por si algo sale mal)
	jmp $

%include "a20.asm"
