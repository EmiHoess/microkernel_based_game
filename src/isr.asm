; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

%define TAREA_QUANTUM		2

exception_msg db		'Exception'
exception_msg_len equ	$ - excepcion_msg

;; PIC
extern fin_intr_pic1


;;
;; Definición de MACROS
;;

%macro ISR 1
global _isr%1

_isr%1:
	; To Infinity And Beyond!!
	jmp $
%endmacro

;;
;; Datos
;;
; Scheduler
reloj_numero:		 	dd 0x00000000
reloj:  				db '|/-\'


;;
;; Rutina de atención de las EXCEPCIONES
;;
ISR 0
imprimir_texto_mp exception_msg, exception_msg_len ,0x07, 0, 0
jmp $

;;
;; Rutina de atención del RELOJ
;;

;;
;; Rutina de atención del TECLADO
;;

;;
;; Rutinas de atención de las SYSCALLS
;;

proximo_reloj:
	pushad

	inc DWORD [reloj_numero]
	mov ebx, [reloj]
	cmp ebx, 0x4
	jl .ok
		mov DWORD [reloj_numero], 0x0
		mov ebx, 0
	.ok:
		add ebx, reloj
		imprimir_texto_mp ebx, 1, 0x0f, 24, 79

	popad
	ret
