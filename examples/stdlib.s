%include "myprintf.s"
section .rodata
output_buffer: db "Result = %h", 10, 0

section .text

out:
        pop r13
        pop rsi
        mov rdi, output_buffer
        push r13
        push rbx
        push rbp
        call MyPrintf
        pop rbp
        pop rbx
        ret
