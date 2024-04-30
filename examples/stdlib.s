section .text

; ---------------------------------
; Scanf for doubles

in:
        pop r13         ; ret address
        push rbx
        push rbp        ; save rbx and rbp

        sub rsp, 32     ; save 32 bytes for buffer
        mov rsi, rsp    ; buffer_ptr
        mov rax, 0
        mov rdi, 0
        mov rdx, 32
        syscall         ; sys read

        add rsp, 32
        xor rax, rax    ; number
        mov rbx, 10     ; mul coeff
        xor rcx, rcx    ; cur_symbol
        xor r8, r8      ; dot
        xor r9, r9      ; minus
        xor rdi, rdi    ; digits_after_dot

.While:
        cmp byte [rsi], '-'
        je .HandleMinus

        cmp byte [rsi], '.'
        je .HandleDot

        cmp byte [rsi], '0'
        jl .Break

        cmp byte [rsi], '9'
        jg .Break

        cmp r8, 0
        je .BeforeDot
        inc rdi

.BeforeDot:
        mov cl, byte [rsi]
        sub cl, '0'             ; cur_digit
        mul rbx                 ; num *= 10
        add rax, rcx            ; num += cur_digit
        inc rsi
        jmp .While

.HandleMinus:
        inc r9
        inc rsi
        jmp .While

.HandleDot:
        inc r8
        inc rsi
        jmp .While

.Break:
        cmp r9, 1
        jne .Positive
        neg rax

.Positive:
        cvtsi2sd xmm1, rax

        cmp r8, 0
        je .EndLabel

        mov rax, 1
.GetBase:
        cmp rdi, 0
        je .LastDigit

        mul rbx
        dec rdi
        jmp .GetBase

.LastDigit:
        cvtsi2sd xmm0, rax
        divsd xmm1, xmm0

.EndLabel:
        sub rsp, 8
        movsd [rsp], xmm1
        pop rax
        pop rbp
        pop rbx
        push r13
        ret

; ------------------------------
; ------------------------------
; Printf for doubles

out:
        pop r13                 ; ret address
        movsd xmm0, [rsp]       ; save value
        add rsp, 8
        push rbx
        push rbp                ; save rbx and rbp

        mov rax, 1000
        cvtsi2sd xmm1, rax
        mulsd xmm0, xmm1
        cvtsd2si rax, xmm0      ; value *= 1000

        xor r8, r8
        cmp rax, 0
        jge .Pozitive
        neg rax
        mov r8, 1

.Pozitive:
        mov rsi, rsp
        sub rsi, 3

        mov rbx, 10             ; base
        xor rcx, rcx            ; n_digits

.While:
        xor rdx, rdx
        div rbx
        add dl, '0'
        mov byte [rsi], dl
        dec rsi
        cmp rax, 0
        jne .While

        mov al, byte [rsp - 3]
        mov ah, byte [rsp - 4]
        mov bl, byte [rsp - 5]

        mov byte [rsp - 5], '.'
        mov byte [rsp - 4], bl
        mov byte [rsp - 3], ah
        mov byte [rsp - 2], al
        mov byte [rsp - 1], 10          ; '\n'

        cmp r8, 1
        jne .NotMinus
        mov byte [rsi], '-'
        dec rsi

.NotMinus:

        inc rsi
        mov rdx, rsp
        sub rdx, rsi
        mov rax, 1
        mov rdi, 1
        syscall

        pop rbp
        pop rbx

        push r13
        ret
