.code

filter proc ;RCX-pixels, RDX-newPixels, R8-processedSize, R9-w
xor eax,eax
mov al, BYTE PTR[rcx + 2137] ;odczytaj wartosc jednego z pixeli(test do usuniecia)
mov BYTE PTR[rcx + 2137], al ;zapisz wartosc jednego z pixeli(test do usuniecia)
xor rax, rax
mov rax, r8
mov rax, r9
ret
filter endp

END