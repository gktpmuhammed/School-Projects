code segment
mov ax, var1
mov bx, 2120d
mul bx
nop
cmp dl, 8d
jne ops
and dh, 11h
jnz ops
mov dl, 'Y'
mov ah, 02h
int 21h
mov ax, 10d
cmp ax, 12d
jbe exit
ops:
    mov dl, 'X'
    mov ah, 02h
    int 21h
exit:
    int 20h
var1 dw 0113h
code ends