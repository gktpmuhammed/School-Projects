code segment
mov ax, 10d
mov bx, 10h
nop
add ax, bx
cmp ax, 26d
je res_26
cmp ax, 20d
je res_20
res_20:
    mov ah,02h
    mov dl, '2'
    int 21h
    mov dl, '0'
    int 21h
    mov ax, 30d
    sub ax, 2h
    cmp ax, 29d
    jnae exit
res_26:
    mov ah,02h
    mov dl, '2'
    int 21h
    mov dl, '6'
    int 21h
    mov ax, 30d
    sub ax, 2h
    cmp ax, 29d
    jnae exit
mov dl, '!'
int 21h
exit:
    int 20h
code ends