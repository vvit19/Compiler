%include "stdlib.s" 
global _start 
section .data 
variables: times 8192 db 0 
section .text 
_start: 
		mov rbx, variables 
		mov rbp, rbx 
		add rbp, 32 
		call main 
		mov eax, 1 
		mov ebx, 0 
		int 80h 
main: 
		pop rsi 
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
		add rbp, 32 
		call factorial 
		sub rbp, 32 
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
factorial: 
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
		push rsi 
		push qword [rbp + 0] 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		add rbp, 32 
		call factorial 
		sub rbp, 32 
		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push qword [rbp + 0] 
		push qword [rbp + 8] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop rax 
		push rsi 
		ret 
