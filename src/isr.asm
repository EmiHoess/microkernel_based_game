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


extern init_game
extern end_game


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

release_p_ve_msg db		'P Key'
release_p_ve_msg_len equ	$ - release_p_ve_msg

release_r_ve_msg db		'R Key'
release_r_ve_msg_len equ	$ - release_r_ve_msg

pause_start: db 0
pause_end: db 0 
quantum: db TAREA_QUANTUM

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

;;
;; Rutina de atención del RELOJ
;;

ISR 32:
	cli
	pushad
	call fin_intr_pic1

	cmp byte [quantum], 0
	jne .finYDec32
		mov byte [quantum], 2 
		cmp byte [pausado], 0 
		jne .noPausar32
			cmp byte [pausar], 1 
			jne .cambiarTarea32
				mov byte [pausado], 1
		;Salto a idle
		jmp 72:00
		jmp .fin32 

	.noPausar32:
		cmp [pausar], 0
		jne .fin32
		mov byte [pausado], 0
		.cambiarTarea32:
		;Pushear registros
		call sched_proximo_indice
		pop ax ;Consigo resultado
		jmp ax:00
		jmp .fin32

	.finYDec32:
		;Decremento el quantum
		mov al, [quantum]
		dec al
		mov [quantum], al
	.fin32:
	call proximo_reloj 
	pop eax
	popfd 				
	iret 				


;;
;; Rutina de atención del TECLADO
;;

ISR 33
	cli
	pushfd
	call fin_intr_pic1
	push eax
	in al, 0x60 
	cmp	al, 93
	jne .pressing_r
	//releasing p
		imprimir_excepcion release_r_ve_msg, release_r_ve_msg_len
		mov [pause_start], 0
		jmp .fin33
	.pressing_r:
	cmp al, 99 
	jne .pressing_p
		imprimir_excepcion release_p_ve_msg, release_p_ve_msg_len
		mov [pause_start], 1
		.fin33:
	.pressing_p:
	pop eax
	popfd
	sti
	iret
;;
;; Rutinas de atención de las SYSCALLS
;;

;;
;; Rutina de atencion x80
;;
ISR 128
pushfd
call fin_intr_pic1

cmp eax,111
Je .duplicar_128
	call obtener_id_jugador 	
	push esi
	push edx
	push ecx
	push ebx
	push eax
	call game_migrar
	add esp,20 
	jmp .salir_128

		.duplicar_128:
		call obtener_id_jugador 	
		push ecx
		push ebx
		push eax
		call game_duplicar 
		add esp,8 			

.salir_128:

popfd 				
iret 			

ISR 144

pushfd
cmp eax, 200
	jne .iniciarJuego
		call end_game
	jmp .fin144
.iniciarJuego:
cmp eax, 300 
	jne .fin144
call init_game
.fin144:
popfd




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
