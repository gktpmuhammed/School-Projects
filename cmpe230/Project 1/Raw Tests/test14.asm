code segment
mov al, 93h
mov ah, 0F5h
add al, ah
jc ex
mov ah, 02h
mov dl, '-'
int 21h
ex:
    mov ah, 02h
    mov dl, '+'
    int 21h
    int 20h
code ends