code segment
read:
    mov ah, 01h
    int 21h
    or init, al
    sub cnt, 1d
    jnz read
print:
    mov dl, init
    mov ah, 02h
    int 21h
    mov dl, init
    xor dl, var2
    int 21h
exit:
    int 20h
init db 62h
var2 db 7h
cnt dw 3d
code ends