.intel_syntax noprefix

.section .text
.global write_char
.global write_int
.global read
.global write
.global read_int
.global read_char

read_int:

    push rdi
    push rsi
    sub rsp, 100

    mov rdi, rsp
    mov rsi, 100
    call read

    mov rdi, rsp
    cmp BYTE PTR [rsp], '-'
    je negative0

    call atoi

    jmp getout0
    negative0:

    add rdi, 1
    call atoi
    neg rax

    getout0:

    add rsp, 100
    pop rsi
    pop rdi

    ret

read_char:

    push rdi
    push rsi
    sub rsp, 100

    mov rdi, rsp
    mov rsi, 100
    call read

    xor rax, rax
    mov al, BYTE PTR[rsp]

    add rsp, 100
    pop rsi
    pop rdi    

    ret

write_char:
    push rdi

    dec rsp

    mov BYTE PTR [rsp], dil

    mov rdi, rsp
    mov rsi, 1
    call write

    inc rsp

    pop rdi

    ret

write_int:

    push rdi
    push rsi

    sub rsp, 100

    cmp rdi, 0
    jl negative

    mov rsi, rsp
    add rsi, 100
    call itoa

    mov rdi, rsp
    add rdi, 100
    sub rdi, rax
    mov rsi, rax
    call write

    jmp cont
    negative:

    neg rdi

    mov rsi, rsp
    add rsi, 100
    call itoa

    mov rdi, rsp
    add rdi, 100
    add rax, 1
    sub rdi, rax
    mov BYTE PTR [rdi], '-'
    mov rsi, rax
    call write

    cont:

    add rsp, 100

    pop rsi
    pop rdi

    ret

itoa:

    push rdx
    push rbx
    push rcx
    push r8

    xor rdx, rdx
    mov rax, rdi
    mov r8, 10

    mov rcx, rsi
    dec rcx

    loopers1:
    xor rdx, rdx

    div r8
    add rdx, '0'
    mov BYTE PTR [rcx], dl
    test rax, rax
    jz getout
    dec rcx
    jmp loopers1

    getout:

    mov rax, rsi
    sub rax, rcx

    pop r8
    pop rcx
    pop rbx
    pop rdx

    ret

atoi:

    push rbx
    push rdx
    push rcx
    push r8
    push r9

    mov rdx, rdi
    mov rcx, 0
    mov rax, -1

    loopers2:
    movzx rbx, BYTE PTR [rdx] 
    cmp bl, '\n'
    je return1
    sub rbx, '0'
    mov rax, -1
    cmp bl, 0
    jb return1
    cmp bl, 9
    ja return1
    mov r8, 922337203685477580
    cmp rcx, r8
    ja return1

    shl rcx, 1
    mov r8, rcx
    shl r8, 2
    add rcx, r8

    mov r8, 9223372036854775807
    sub r8, rbx

    cmp rcx, r8
    ja return1

    add rcx, rbx
    mov rax, rcx

    add rdx, 1
    jmp loopers2

    return1:

    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rbx

    ret

#############################

write:

    push rdx
    push rsi
    push rax
    push rdi
    mov rdx, rsi
    mov rsi, rdi
    mov rax, 1
    mov rdi, 1
    syscall
    pop rdi
    pop rax
    pop rsi
    pop rdx

    ret

#############################

read:

    push rdx
    push rsi
    push rax
    push rdi
    mov rdx, rsi
    mov rsi, rdi
    mov rax, 0
    mov rdi, 1
    syscall
    pop rdi
    pop rax
    pop rsi
    pop rdx

    ret
    