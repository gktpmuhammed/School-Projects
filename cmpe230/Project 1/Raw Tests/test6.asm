code segment
mov ax, 40d
nop
mov bx, var1
push bx
mov cx, var2
push cx
push ax
pop bx
cmp bx, 28h
jne hey
pop bx
cmp sp, 0FFFCh
je cor
hey:
    mov ah, 02h
    mov dl,     'X'
    int 21h
cor:
    mov ah, 02h
    mov dl, '+'
    int 21h
int 20h
var1 dw 10h
var2 dw 20h
code ends