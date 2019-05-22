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

global jmp_to_task
extern init_game
extern end_game
extern sched_next_index
extern game_tick
extern game_duplicar
extern game_migrar
extern current_task
extern set_memory
extern sched_remove_task
extern spend_turn

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

%macro JUST_SPEND_TURN 2
	global _isr%1

	%defstr intr_num_%1 %1
	intr_msg_%1     db  %2, ' (', intr_num_%1, ')', 0
	intr_msg_len_%1 equ $ - intr_msg_%1

	_isr%1:

	pushfd
	pushad

	call current_task 			

	eliminar_tarea_actual eax

 	mensaje_tarea eax, intr_msg_%1

	popad
	popfd

	call spend_turn
%endmacro

ISR 14
pushfd
pushad
mov eax, cr2 
push eax
call set_memory
add esp, 4 
cmp ax, 0 
	jne .fin14
	call current_task 
	sub ax, 10d 
	push ax 
	call sched_remove_task
	add esp, 4 
.fin14:
popad
popfd
iret

JUST_SPEND_TURN  0, '#DE Divide Error'
JUST_SPEND_TURN  1, '#DB RESERVED'
JUST_SPEND_TURN  2, 'NMI Interrupt'
JUST_SPEND_TURN  3, '#BP Breakpoint'
JUST_SPEND_TURN  4, '#OF Overflow'
JUST_SPEND_TURN  5, '#BR BOUND Range Exceeded'
JUST_SPEND_TURN  6, '#UD Invalid Opcode (Undefined Opcode)'
JUST_SPEND_TURN  7, '#NM Device Not Available (No Math Coprocessor)'
JUST_SPEND_TURN  8, '#DF Double Fault'
JUST_SPEND_TURN  9, 'Coprocessor Segment Overrun (reserved)'
JUST_SPEND_TURN 10, '#TS Invalid TSS'
JUST_SPEND_TURN 11, '#NP Segment Not Present'
JUST_SPEND_TURN 12, '#SS Stack-Segment Fault'
JUST_SPEND_TURN 13, '#GP General Protection'
JUST_SPEND_TURN 15, '(Intel reserved. Do not use.)'
JUST_SPEND_TURN 16, '#MF x87 FPU Floating-Point Error (Math Fault)'
JUST_SPEND_TURN 17, '#AC Alignment Check'
JUST_SPEND_TURN 18, '#MC Machine Check'
JUST_SPEND_TURN 19, '#XM SIMD Floating-Point Exception'
JUST_SPEND_TURN 20, '#VE Virtualization Exception'


jmp_to_task:
	push ebp
	mov ebp, esp
	pushad
	xchg bx, bx
	mov eax, [ebp+8]
	mov [proximaTarea], ax
    jmp far [offset]
    popad
   	pop ebp
    ret

;;
;; Rutina de atención del RELOJ
;;

ISR 32:
	pushfd 					
	call fin_intr_pic1 	
	call game_tick			
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
	pushad
	call fin_intr_pic1 	;
	popad
	xchg bx, bx
	
	
	cmp eax, 111
	je .duplicar_128

		cmp eax, 222
		je .migrar_128

			mov eax, 0
			jmp .salir_128

	.duplicar_128:
	
	push ecx 			
	push ebx 			
	call current_player 	
	pop ebx
	pop ecx

					
	push ecx 			
	push ebx 			
	push eax 			
	call game_duplicar 
	add esp, 12

	jmp .salir_128

	.migrar_128:
	push ecx 			
	push ebx 			
	call current_player 	
	pop ecx
	pop ebx
	
	push esi 			
	push edx 			
	push ecx 			
	push ebx 			
	push eax 			
	call game_migrar
	add esp, 20

	.salir_128:
	cmp eax, 0
		je .fin_128
		pushad
		call spend_turn
		popad
	.fin_128:
	popfd 				
	iret 				





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
