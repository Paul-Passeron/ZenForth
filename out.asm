format ELF64 executable 3
segment readable executable
dump:
        mov     r9, -3689348814741910323
        sub     rsp, 40
        mov     BYTE [rsp+31], 10
        lea     rcx, [rsp+30]
.L2:
        mov     rax, rdi
        lea     r8, [rsp+32]
        mul     r9
        mov     rax, rdi
        sub     r8, rcx
        shr     rdx, 3
        lea     rsi, [rdx+rdx*4]
        add     rsi, rsi
        sub     rax, rsi
        add     eax, 48
        mov     BYTE [rcx], al
        mov     rax, rdi
        mov     rdi, rdx
        mov     rdx, rcx
        sub     rcx, 1
        cmp     rax, 9
        ja      .L2
        lea     rax, [rsp+32]
        mov     edi, 1
        sub     rdx, rax
        lea     rsi, [rsp+32+rdx]
        mov     rdx, r8
        mov     rax, 1
        syscall
        add     rsp, 40
        ret
proc_at:
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    push 16
    ;; MUL
    pop rdx
    pop rax
    mul rdx
    push rax
    ;; SUB
    pop rax
    pop rbx
    sub rbx, rax
    push rbx
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rbx, [scope_sizes_rsp]
    add rax, [rbx]
    mov rbx, [scope_sizes_rsp]
    add rax, [rbx]
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    add rsp, 0
    ret
proc_inc:
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    push 1
    ;; ADD
    pop rax
    pop rbx
    add rbx, rax
    push rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rbx, [scope_sizes_rsp]
    add rax, [rbx]
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    add rsp, 0
    ret
proc_print_array:
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 32
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    push 0
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; WHILE
.DO0:
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; LESS
    pop rax
    pop rbx
    cmp rbx, rax
    mov rcx, 0
    mov rdx, 1
    cmovl rcx, rdx
    push rcx
    ;; DO
    pop rax
    cmp rax, 0
    je .WHILE0
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 32
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; CALL at
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_at
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PRINT
    pop rdi
    call dump
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; CALL inc
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_inc
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    jmp .DO0
.WHILE0:
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    add rsp, 0
    ret
proc_main:
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; PUSH
    push 5
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    push 5
    ;; Declare local variable
    pop rdx
    push rbx
    mov rbx, [scope_stack_rsp]
    mov rax, 16
    mul rdx
    add rbx, rax
    mov [scope_stack_rsp], rbx
    pop rbx
    ;; Declare local variable
    push rbx
    mov rbx, [scope_stack_rsp]
    add rbx, 16
    mov [scope_stack_rsp], rbx
    mov rbx, [scope_sizes_rsp]
    mov [scope_sizes_rsp]
    add rbx,  1
    mov [scope_sizes_rsp], rbx
    pop rbx
    ;; PUSH
    push 0
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; WHILE
.DO0:
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 96
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; LESS
    pop rax
    pop rbx
    cmp rbx, rax
    mov rcx, 0
    mov rdx, 1
    cmovl rcx, rdx
    push rcx
    ;; DO
    pop rax
    cmp rax, 0
    je .WHILE0
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; PUSH
    push 2
    ;; MUL
    pop rdx
    pop rax
    mul rdx
    push rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; CALL at
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_at
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; STORE
    pop rbx
    pop rax
    mov [rbx], rax
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 0
    push rbx
    ;; CALL inc
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_inc
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    jmp .DO0
.WHILE0:
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 16
    push rbx
    ;; PUSH
    mov rbx, [scope_stack_rsp]
    sub rbx, 96
    push rbx
    ;; FETCH
    pop rbx
    mov rax, [rbx]
    push rax
    ;; CALL print_array
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_print_array
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; Deallocate local variables
    push rbx
    push rax
    mov rax, 0
    mov rdx, 16
    mul rdx
    mov rbx, [scope_stack_rsp]
    sub rbx, rax
    mov [scope_stack_rsp], rbx
    pop rax
    pop rbx
    ;; END
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    add rsp, 0
    ret

entry start
start:
    mov rax, ret_stack_end
    mov [ret_stack_rsp], rax
    mov rax, scope_sizes
    mov [scope_sizes_rsp], rax
    mov rax, scope_stack
    mov [scope_stack_rsp], rax
    ;; CALL MAIN
    mov rax, rsp
    mov rsp, [ret_stack_rsp]
    call proc_main
    mov [ret_stack_rsp], rsp
    mov rsp, rax
    ;; EXIT PROGRAM
    mov rax, 60
    mov rdi, 0
    syscall
segment readable writable
ret_stack_rsp: rq 1
ret_stack: rb 65536
ret_stack_end:
scope_stack_rsp: rq 1
scope_stack: rb 16000
scope_stack_end:
scope_sizes_rsp: rq 1
scope_sizes: rb 16000
scope_sizes_end:
