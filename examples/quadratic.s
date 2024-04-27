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
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 0] 
		sub rsp, 8 
		mov rcx, 0x4008000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 8] 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 16] 
		sub rsp, 8 
		mov rcx, 0x4022000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 24] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop qword [rbp + 32] 
		push qword [rbp + 24] 
		movsd xmm1, qword [rsp] 
		sqrtsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbp + 32] 
		push rsi 
		push qword [rbp + 32] 
		call out 
		pop rsi 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop qword [rbx + 0] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop qword [rbx + 8] 
		push rsi 
		push qword [rbp + 16] 
		push qword [rbp + 8] 
		push qword [rbp + 0] 
		add rbp, 50 
		call solve_quadratic 
		sub rbp, 50 
		pop rsi 
		push rax 
		pop qword [rbp + 40] 
		push rsi 
		push qword [rbp + 40] 
		call out 
		pop rsi 
		push qword [rbp + 40] 
		sub rsp, 8 
		mov rcx, 0x4020000000000000 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_0 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_0: 
		push qword [rbp + 40] 
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
		push qword [rbp + 40] 
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
		add rbp, 50 
		call solve_linear 
		sub rbp, 50 
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
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		sub rsp, 8 
		mov rcx, 0x4010000000000000 
		mov qword [rsp], rcx 
		push qword [rbp + 0] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		push qword [rbp + 16] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbp + 32] 
		push qword [rbp + 32] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jbe L_4 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_4: 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		push qword [rbp + 8] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		push qword [rbp + 32] 
		movsd xmm1, qword [rsp] 
		sqrtsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		addsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		push qword [rbp + 0] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbx + 0] 
		push qword [rbp + 32] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_5 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_5: 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		push qword [rbp + 8] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		push qword [rbp + 32] 
		movsd xmm1, qword [rsp] 
		sqrtsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		sub rsp, 8 
		mov rcx, 0x4000000000000000 
		mov qword [rsp], rcx 
		push qword [rbp + 0] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		mulsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbx + 8] 
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
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_6 
		push qword [rbp + 8] 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rcx 
		pop rdx 
		cmp rcx, rdx 
		jne L_7 
		sub rsp, 8 
		mov rcx, 0x4020000000000000 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
		jmp L_8 
L_7: 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_8: 
		jmp L_9 
L_6: 
		sub rsp, 8 
		mov rcx, 0x0 
		mov qword [rsp], rcx 
		push qword [rbp + 8] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		subsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		push qword [rbp + 0] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		divsd xmm0, xmm1 
		movsd qword [rsp], xmm0 
		pop qword [rbx + 0] 
		sub rsp, 8 
		mov rcx, 0x3ff0000000000000 
		mov qword [rsp], rcx 
		pop rax 
		push rsi 
		ret 
L_9: 
		push qword [rbp + -8] 
		pop rax 
		push rsi 
		ret 
