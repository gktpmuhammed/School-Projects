code segment
print_cm:
   mov ah, 02h
   mov cx,offset dat1
   mov bp,cx
   add bp, 2
   sub bp, 1
   mov dl,b[bp]
   int 21h
   sub bp, 1
   mov dl, b[ bp ]
   int 21h
print_pe:
   mov dx, offset dat2
   mov bp, dx
   add bp, 2
   sub bp, 1
   mov dl,b[bp]
   int 21h
   sub bp, 1
   mov dl,  b [ bp  ]
   int   21h
int 20h
dat1 dw 436dh
dat2 dw 7045h    
code ends