global _start 
section .data 
buffer: times 2500 dq 0 
section .text 
_start: 
		mov rbx, buffer 
		mov rbp, rbx 
		add rbp, 50 
		call main 
		mov eax, 1 
		mov ebx, 0 
		int 80h 
main: 
		pop rsi 
		sub rsp, 8 
		mov rcx, 0x4014000000000000 
		mov qword [rsp], rcx 
		pop qword [rbp + 0] 
		push rsi 
		push qword [rbp + 0] 
		add rbp, 50 
		call factorial 
		sub rbp, 50 
		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push qword [rbp + 8] 
		call out 
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
		pop rcx
		subsd xmm0, xmm1 
		sub rsp, 8 
		movsd qword [rsp], xmm1 
		add rbp, 50 
		call factorial 
		sub rbp, 50 
		pop rsi 
		push rax 
		pop qword [rbp + 8] 
		push qword [rbp + 0] 
		push qword [rbp + 8] 
		movsd xmm0, qword [rsp + 8] 
		movsd xmm1, qword [rsp] 
		pop rcx
		pop rcx
		mulsd xmm0, xmm1 
		sub rsp, 8 
		movsd qword [rsp], xmm1 
		pop rax 
		push rsi 
		ret 
