code segment
mov bl, 91h
mov bh, 0DDh
sub bl, bh
jnc ex
mov ah, 02h
mov dl, '0'
int 21h
mov al, 93h
mov ah, 065h
add al, ah
jnc ex
mov ah, 02h
mov dl, 'X'
int 21h
ex:
    mov ah, 02h
    mov dl, '1'
    int 21h
    int 20h
code ends