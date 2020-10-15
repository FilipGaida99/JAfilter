.code

filter proc ;RCX-pixels, RDX-newPixels, R8-dataSize, R9-w
push rbx ;zachowanie rbx na stosie.
xor rbx,rbx ;wyzerowanie RBX s³u¿¹cego jako licznik.
xorps xmm0, xmm0 ; wyzerowanie xmm0 slu¿¹cego do konwersji spakowanych Byte na Word
xor r10, r10 ;wyzerowanie r10 s³u¿¹cego jako przesuniêcie na bitmapie w dó³
sub r10, r9 ;przypisanie do r10 ujemnej wartoœci szerokoœci
przetwarzanie_wektora: 
movups xmm1, [rcx]
movaps xmm2, xmm1 ;xmm2 i xmm1 - dane wejsciowe
punpckhbw xmm1, xmm0 ;xmm1 - starsze bajty danych wejsciowych odczytywane jako s³owa.
punpcklbw xmm2, xmm0 ;xmm2 - m³odsze bajty danych wejsciowych odczytywane jako s³owa.

movups xmm3, [rcx - 3] ;wektor s¹siadów z lewej
movaps xmm4, xmm3
punpckhbw xmm3, xmm0 ;xmm3 - starsze bajty s¹siadów odczytywane jako s³owa.
punpcklbw xmm4, xmm0 ;xmm3 - starsze bajty s¹siadów odczytywane jako s³owa.
paddw xmm1, xmm3 ;dodanie wartoœci zgodnie z zasotoswanymi wagami
paddw xmm2, xmm4 
movups xmm3, [rcx + 3] ;wektor s¹siadów z prawej
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 ;odejmowanie wartoœci zgodnie z zasotoswanymi wagami
psubw xmm2, xmm4 

movups xmm3, [rcx + r10 - 3] ;wektor s¹siadów z lewgo dolnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
paddw xmm1, xmm3 ;dodanie wartoœci zgodnie z zasotoswanymi wagami
paddw xmm2, xmm4 
movups xmm3, [rcx + r10 + 3] ;wektor s¹siadów z prawego dolnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 ;odejmowanie wartoœci zgodnie z zasotoswanymi wagami
psubw xmm2, xmm4 

movups xmm3, [rcx + r9 - 3] ;wektor s¹siadów z lewgo górnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
paddw xmm1, xmm3 
paddw xmm2, xmm4 ;dodanie wartoœci zgodnie z zasotoswanymi wagami
movups xmm3, [rcx + r9 + 3] ;wektor s¹siadów z prawego górnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 
psubw xmm2, xmm4 ;odejmowanie wartoœci zgodnie z zasotoswanymi wagami
;pomijam dolnych i górnych s¹siadów, poniewa¿ nie wp³ywaj¹ na wynik.

packuswb xmm2, xmm1 ;ponowne spakowanie s³ów do bajtów. Z nasyceniem bez znaku.
movups [rdx], xmm2 ;zapisanie wyniku w tablicy wynikowej.

add rbx,16 ;zwiêkszenie licznika o liczbê przetworzonych bajtów.
add rcx,16 ;przesuniêcie wskaznika tablicy wejœciowej za przetworzone bajty.
add rdx,16 ;przesuniêcie wskaznika tablicy wyjœciowej za zapisane bajty.
cmp rbx,r8 ;warunek koñca pêtli. licznik < dataSize
jl przetwarzanie_wektora
pop rbx ;powrót rbx do stanu z przed uruchomienia funkcji.
ret
filter endp

END