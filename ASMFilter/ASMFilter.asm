.code

filter proc ;RCX-pixels, RDX-newPixels, R8-dataSize, R9-w
push rbx ;zachowanie rbx na stosie.
xor rbx,rbx ;wyzerowanie RBX s³u¿¹cego jako licznik.
xorps xmm1, xmm1 ; wyzerowanie Xmm1 slu¿¹cego do konwersji spakowanych Byte na Word
przetwarzanie: 
movups xmm0, [rcx + rbx]
movaps xmm2, xmm0 ;xmm2 i xmm0 - dane wejsciowe
punpckhbw xmm0, xmm1 ;xmm0 - starsze bajty danych wejsciowych odczytywane jako s³owa.
punpcklbw xmm2, xmm1 ;xmm2 - m³odsze bajty danych wejsciowych odczytywane jako s³owa.
packuswb xmm2, xmm0 ;ponowne spakowanie s³ów do bajtów.
movups [rdx + rbx], xmm2
add rbx,16
cmp rbx,r8
jl przetwarzanie
xor rax, rax
mov rax, r8
mov rax, r9
pop rbx ;powrót rbx do stanu z przed uruchomienia funkcji.
ret
filter endp

END