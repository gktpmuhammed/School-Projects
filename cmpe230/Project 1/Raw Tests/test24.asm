code segment
mov ax,0F8h
mov bx,7
not bl
xor ax,bx
jnz fin
mov cx,10
mov bl,1
mov dl,65d
myloop:
mov ah,2
int 21h
add dl,bl
sub cl,bl
cmp cl,bl
jae myloop                              
fin:
int 20h
code ends