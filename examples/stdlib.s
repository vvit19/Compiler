%include "myprintf.s"
section .rodata
output_buffer: db "Result = %d", 10, 0
section .text

out:
        pop r13
        movsd xmm1, qword [rsp]
        pop rsi
        cvtsd2si esi, xmm1
        mov rdi, output_buffer
        push r13
        push rbx
        push rbp
        call MyPrintf
        pop rbp
        pop rbx
        ret
