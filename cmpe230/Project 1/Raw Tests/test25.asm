code segment
mov ax,59
mov bx,89
mov si,420h
mov w[si],bx
mov cx,202h
or al,bl
and al,b[si]
or al,cl
shl ax,3
mov dl,al
mov dh,0
shr dx,1
and dl,bl
rcr dl,1
int 21h
int 20h
code ends