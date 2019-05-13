; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

%macro imprimir_excepcion 2
	mov ecx, 0x7FFF
	.set_zeros:
		mov byte [fs:ecx], 0
		loop .set_zeros
	imprimir_texto_mp excepcion_msg, excepcion_msg_len ,0x07, 0, 0
	imprimir_texto_mp %1, %2, 0x06, 1 ,0
%endmacro

BITS 32

%define TAREA_QUANTUM		2
exception_msg db		'Exception:'
excepcion_msg_len equ	$ - exception_msg

exception_division_msg db		'Zero div'
exception_division_msg_len equ	$ - exception_division_msg

exception_debug_msg db		'Debug'
exception_debug_msg_len equ	$ - exception_debug_msg

exception_nmi_msg db		'NMI'
exception_nmi_msg_len equ	$ - exception_nmi_msg

exception_bp_msg db			'Breakpoint'
exception_bp_msg_len equ	$ - exception_bp_msg

exception_of_msg db		'Overflow'
exception_of_msg_len equ	$ - exception_of_msg

exception_br_msg db		'BRange Exceeded'
exception_br_msg_len equ	$ - exception_br_msg

exception_ud_msg db		'Invalid Opcode'
exception_ud_msg_len equ	$ - exception_ud_msg

exception_nm_msg db		'Device Not Available'
exception_nm_msg_len equ	$ - exception_nm_msg

exception_df_msg db		'Double Fault'
exception_df_msg_len equ	$ - exception_df_msg

exception_cpso_msg db		'Coprocesor Segment Overrun'
exception_cpso_msg_len equ	$ - exception_cpso_msg

exception_ts_msg db		'Invalid TSS'
exception_ts_msg_len equ	$ - exception_ts_msg

exception_np_msg db		'Segment Not Present'
exception_np_msg_len equ	$ - exception_np_msg

exception_ss_msg db		'Stack Fault'
exception_ss_msg_len equ	$ - exception_ss_msg

exception_gp_msg db		'General Protection'
exception_gp_msg_len equ	$ - exception_gp_msg

exception_pf_msg db		'Page Fault'
exception_pf_msg_len equ	$ - exception_pf_msg

exception_mf_msg db		'FPU Floating point Error'
exception_mf_msg_len equ	$ - exception_mf_msg

exception_ac_msg db		'Alignment Check'
exception_ac_msg_len equ	$ - exception_ac_msg

exception_mc_msg db		'Maching Check'
exception_mc_msg_len equ	$ - exception_mc_msg

exception_xm_msg db		'SIMD Floating point'
exception_xm_msg_len equ	$ - exception_xm_msg

exception_ve_msg db		'Virtualization'
exception_ve_msg_len equ	$ - exception_ve_msg


;; PIC
extern fin_intr_pic1

%macro ISR 1

global _isr%1

_isr%1:
	; To Infinity And Beyond!!
	;jmp $

%endmacro

;;
;; Datos
;;
; Scheduler
reloj_numero:		 	dd 0x00000000
reloj:  				db '|/-\'

;;
;; Rutina de atención de las exceptionES
;;

ISR 0
print_exception exception_division_msg, exception_division_msg_len
jmp $

ISR 1
print_exception exception_debug_msg, exception_debug_msg_len
jmp $

ISR 2
print_exception exception_nmi_msg, exception_nmi_msg_len
jmp $

ISR 3
print_exception exception_bp_msg, exception_bp_msg_len
jmp $

ISR 4
print_exception exception_of_msg, exception_of_msg_len
jmp $

ISR 5
print_exception exception_br_msg, exception_br_msg_len
jmp $

ISR 6
print_exception exception_ud_msg, exception_ud_msg_len
jmp $

ISR 7
print_exception exception_nm_msg, exception_nm_msg_len
jmp $

ISR 8
print_exception exception_df_msg, exception_df_msg_len
jmp $

ISR 9
print_exception exception_cpso_msg, exception_cpso_msg_len
jmp $

ISR 10
print_exception exception_ts_msg, exception_ts_msg_len
jmp $

ISR 11
print_exception exception_np_msg, exception_np_msg_len
jmp $

ISR 12
print_exception exception_ss_msg, exception_ss_msg_len
jmp $

ISR 13
print_exception exception_gp_msg, exception_gp_msg_len
jmp $

ISR 14
print_exception exception_pf_msg, exception_pf_msg_len
jmp $

ISR 15
;print_exception exception_mr_msg, exception_mr_msg_len
jmp $

ISR 16
print_exception exception_mf_msg, exception_mf_msg_len
jmp $

ISR 17
print_exception exception_ac_msg, exception_ac_msg_len
jmp $

ISR 18
print_exception exception_mc_msg, exception_mc_msg_len
jmp $

ISR 19
print_exception exception_xm_msg, exception_xm_msg_len
jmp $

ISR 20
print_exception exception_ve_msg, exception_ve_msg_len
jmp $

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