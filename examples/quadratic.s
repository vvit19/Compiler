%include "stdlib.s" 
global _start 
section .data 
variables: times 8192 db 0 
section .text 
_start: 
		mov rbx, variables 
		mov rbp, rbx 
		add rbp, 256 
		call main 
		mov eax, 1 
		mov ebx, 0 
		int 80h 
main: 
		pop rsi 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbp + 8] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbp + 16] 
		push rsi 
		call in 
		pop rsi 
		push rax 
		pop qword [rbp + 0] 
		push rsi 
		call in 
		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push rsi 
		call in 
		pop rsi 
		push rax 
		pop qword [rbp + 16] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbx + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbx + 8] 
		push rsi 
		push qword [rbp + 16] 
		push qword [rbp + 8] 
		push qword [rbp + 0] 

; Call: 
		add rbp, 256 
		call solve_quadratic 
		sub rbp, 256 

		pop rsi 
		push rax 
		pop qword [rbp + 24] 
		push rsi 
		push qword [rbp + 24] 
		call out 
		pop rsi 
		push qword [rbp + 24] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4020000000000000 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_0 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_0: 
		push qword [rbp + 24] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		je L_1  
		push rsi 
		push qword [rbx + 0] 
		call out 
		pop rsi 
L_1: 
		push qword [rbp + 24] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_2 
		push rsi 
		push qword [rbx + 8] 
		call out 
		pop rsi 
L_2: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
solve_quadratic: 
		pop rsi 
		pop qword [rbp + 0] 
		pop qword [rbp + 8] 
		pop qword [rbp + 16] 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_3 
		push rsi 
		push qword [rbp + 16] 
		push qword [rbp + 8] 

; Call: 
		add rbp, 256 
		call solve_linear 
		sub rbp, 256 

		pop rsi 
		push rax 
		pop qword [rbp + 24] 
		push qword [rbp + 24] 
		pop rax 
		push rsi 
		ret 
L_3: 
		push qword [rbp + 8] 
		push qword [rbp + 8] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4010000000000000 
		mov qword [rsp], rcx 

		push qword [rbp + 0] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		push qword [rbp + 16] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbp + 32] 
		push qword [rbp + 32] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jle L_4 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_4: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		push qword [rbp + 8] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		push qword [rbp + 32] 
; Sqrt: 
		movsd xmm1, qword [rsp] 
		sqrtsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		addsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 

		push qword [rbp + 0] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbx + 0] 
		push qword [rbp + 32] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_5 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_5: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		push qword [rbp + 8] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		push qword [rbp + 32] 
; Sqrt: 
		movsd xmm1, qword [rsp] 
		sqrtsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 

		push qword [rbp + 0] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbx + 8] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
solve_linear: 
		pop rsi 
		pop qword [rbp + 0] 
		pop qword [rbp + 8] 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_6 
		push qword [rbp + 8] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_7 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4020000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
		jmp L_8 
L_7: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_8: 
		jmp L_9 
L_6: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		push qword [rbp + 8] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		push qword [rbp + 0] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbx + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_9: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
