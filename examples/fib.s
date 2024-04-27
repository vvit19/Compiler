%include "stdlib.s" 
global _start 
section .data 
variables: times 2500 dq 0 
section .text 
_start: 
		mov rbx, variables 
		mov rbp, rbx 
		add rbp, 50 
		call main 
		mov eax, 1 
		mov ebx, 0 
		int 80h 
main: 
		pop rsi 
		sub rsp, 8 
		mov rcx, 0x4018000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 0] 
		push rsi 
		push qword [rbp + 0] 
		add rbp, 50 
		call fib 
		sub rbp, 50 
		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push rsi 
		push qword [rbp + 8] 
		call out 
		pop rsi 
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
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_0 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_0: 
		push qword [rbp + 0] 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_1 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_1: 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 8] 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 16] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop qword [rbp + 24] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop qword [rbp + 32] 
		push qword [rbp + 32] 
		push qword [rbp + 0] 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jbe L_2 
L_3: 
		push qword [rbp + 8] 
		push qword [rbp + 16] 
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
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		addsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbp + 32] 
		push qword [rbp + 32] 
		push qword [rbp + 0] 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jbe L_2 
		jmp L_3 
L_2: 
		push qword [rbp + 24] 
		pop rax 
		push rsi 
		ret 
