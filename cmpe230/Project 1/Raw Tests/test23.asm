code segment
mov dx,0
mov bx,3
mov cx,4
push bx
mov bx,0DEFAh
mov ax,sp
div bx
mov ah,2
mov dl,dh
int 21h
push cx
mov dx,sp
sub dx,198
mov ah,2
int 21h
pop dx
add dx,48
mov ah,2
int 21h
int 20h
code ends