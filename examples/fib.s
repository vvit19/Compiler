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
		push rsi 
		call in 
		pop rsi 
		push rax 
		pop qword [rbp + 0] 
		push rsi 
		push qword [rbp + 0] 

; Call: 
		add rbp, 256 
		call fib 
		sub rbp, 256 

		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push rsi 
		push qword [rbp + 8] 
		call out 
		pop rsi 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
fib: 
		pop rsi 
		pop qword [rbp + 0] 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_0 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_0: 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_1 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop rax 
		push rsi 
		ret 
L_1: 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop qword [rbp + 8] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 

		pop qword [rbp + 16] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbp + 24] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 

		pop qword [rbp + 32] 
		push qword [rbp + 32] 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jle L_2 
L_3: 
		push qword [rbp + 8] 
		push qword [rbp + 16] 

; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		addsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbp + 24] 
		push qword [rbp + 16] 
		pop qword [rbp + 8] 
		push qword [rbp + 24] 
		pop qword [rbp + 16] 
		push qword [rbp + 32] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		addsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop qword [rbp + 32] 
		push qword [rbp + 32] 
		push qword [rbp + 0] 

; Push imm (double) 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 


; Math operation:
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 

		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jle L_2 
		jmp L_3 
L_2: 
		push qword [rbp + 24] 
		pop rax 
		push rsi 
		ret 
