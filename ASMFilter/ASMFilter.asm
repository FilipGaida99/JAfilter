.code

filter proc ;RCX-pixels, RDX-newPixels, R8-dataSize, R9-w
push rbx ;zachowanie rbx na stosie.
xor rbx,rbx ;wyzerowanie RBX s�u��cego jako licznik.
xorps xmm0, xmm0 ; wyzerowanie xmm0 slu��cego do konwersji spakowanych Byte na Word
xor r10, r10 ;wyzerowanie r10 s�u��cego jako przesuni�cie na bitmapie w d�
sub r10, r9 ;przypisanie do r10 ujemnej warto�ci szeroko�ci
przetwarzanie_wektora: 
movups xmm1, [rcx]
movaps xmm2, xmm1 ;xmm2 i xmm1 - dane wejsciowe
punpckhbw xmm1, xmm0 ;xmm1 - starsze bajty danych wejsciowych odczytywane jako s�owa.
punpcklbw xmm2, xmm0 ;xmm2 - m�odsze bajty danych wejsciowych odczytywane jako s�owa.

movups xmm3, [rcx - 3] ;wektor s�siad�w z lewej
movaps xmm4, xmm3
punpckhbw xmm3, xmm0 ;xmm3 - starsze bajty s�siad�w odczytywane jako s�owa.
punpcklbw xmm4, xmm0 ;xmm3 - starsze bajty s�siad�w odczytywane jako s�owa.
paddw xmm1, xmm3 ;dodanie warto�ci zgodnie z zasotoswanymi wagami
paddw xmm2, xmm4 
movups xmm3, [rcx + 3] ;wektor s�siad�w z prawej
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 ;odejmowanie warto�ci zgodnie z zasotoswanymi wagami
psubw xmm2, xmm4 

movups xmm3, [rcx + r10 - 3] ;wektor s�siad�w z lewgo dolnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
paddw xmm1, xmm3 ;dodanie warto�ci zgodnie z zasotoswanymi wagami
paddw xmm2, xmm4 
movups xmm3, [rcx + r10 + 3] ;wektor s�siad�w z prawego dolnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 ;odejmowanie warto�ci zgodnie z zasotoswanymi wagami
psubw xmm2, xmm4 

movups xmm3, [rcx + r9 - 3] ;wektor s�siad�w z lewgo g�rnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
paddw xmm1, xmm3 
paddw xmm2, xmm4 ;dodanie warto�ci zgodnie z zasotoswanymi wagami
movups xmm3, [rcx + r9 + 3] ;wektor s�siad�w z prawego g�rnego rogu.
movaps xmm4, xmm3
punpckhbw xmm3, xmm0
punpcklbw xmm4, xmm0
psubw xmm1, xmm3 
psubw xmm2, xmm4 ;odejmowanie warto�ci zgodnie z zasotoswanymi wagami
;pomijam dolnych i g�rnych s�siad�w, poniewa� nie wp�ywaj� na wynik.

packuswb xmm2, xmm1 ;ponowne spakowanie s��w do bajt�w. Z nasyceniem bez znaku.
movups [rdx], xmm2 ;zapisanie wyniku w tablicy wynikowej.

add rbx,16 ;zwi�kszenie licznika o liczb� przetworzonych bajt�w.
add rcx,16 ;przesuni�cie wskaznika tablicy wej�ciowej za przetworzone bajty.
add rdx,16 ;przesuni�cie wskaznika tablicy wyj�ciowej za zapisane bajty.
cmp rbx,r8 ;warunek ko�ca p�tli. licznik < dataSize
jl przetwarzanie_wektora
pop rbx ;powr�t rbx do stanu z przed uruchomienia funkcji.
ret
filter endp

END