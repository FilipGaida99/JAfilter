.code

filter proc ;
xor eax,eax
pop ax
mov rcx, [rcx] ;Uzyskaj wska�nik z referencji do nowych pikseli.
mov al, BYTE PTR[rcx + 2137]
ret
filter endp

END