code segment
mov ah, var1
mov bx, var2
nop
mov ah, var2
mov bx, var1
int 20h
var1 db 10h
var2 dw 20h
code ends