code segment
mov cx, 3
inputs:
    mov ah, 01h
    int 21h
    mov ah, 0h
    push ax
    sub cx, 1
    cmp cx, 0
    jnz inputs
    mov bx, 3
print:
    mov ah,02h
    pop dx
    int 21h
    sub bx, 1
    cmp bx, 0
    jne print
ex:
    int 20h
code ends