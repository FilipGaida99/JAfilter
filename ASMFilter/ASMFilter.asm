;Temat: Filtrowanie uwypuklaj�ce skierowane na zach�d.
;Algorytm b�dzie filtrowa� ka�dy z kolor�w pixeli zgodnie z mask�
;1 0 -1
;1 1 -1
;1 0 -1
;Wykona�: Filip Gaida gr.3 semestr 5, 2020/2021
;Algortym w tym pliku by� podstawowo napisany pod rejestry xmm, jednak dla test�w rozszerzy�em go do rejestr�w ymm
.code

filter proc ;RCX-pixels, RDX-newPixels, R8-dataSize, R9-w
xor r11,r11 ;wyzerowanie r11 s�u��cego jako licznik.
vxorps ymm0, ymm0, ymm0 ; wyzerowanie ymm0 slu��cego do konwersji spakowanych Byte na Word.
xor r10, r10 ;wyzerowanie r10 s�u��cego jako przesuni�cie na bitmapie w d�.
sub r10, r9 ;przypisanie do r10 ujemnej warto�ci szeroko�ci.
przetwarzanie_wektora: 
vmovdqu ymm1, YMMWORD PTR[rcx] ;ymm1 - dane wejsciowe.
vmovdqu ymm7, YMMWORD PTR[rcx + r10 - 3] ;wektor s�siad�w z lewgo dolnego rogu.
vmovdqu ymm9, YMMWORD PTR[rcx + r10 + 3] ;wektor s�siad�w z prawego dolnego rogu.
vmovdqu ymm3, YMMWORD PTR[rcx - 3] ;wektor s�siad�w z lewej.
vmovdqu ymm5, YMMWORD PTR[rcx + 3] ;wektor s�siad�w z prawej.
vmovdqu ymm11, YMMWORD PTR[rcx + r9 - 3] ;wektor s�siad�w z lewgo g�rnego rogu.
vmovdqu ymm13, YMMWORD PTR[rcx + r9 + 3] ;wektor s�siad�w z lewgo g�rnego rogu.

vpunpcklbw ymm2, ymm1, ymm0 ;ymm2 - m�odsze bajty danych wejsciowych odczytywane jako s�owa.
vpunpckhbw ymm1, ymm1, ymm0 ;ymm1 - starsze bajty danych wejsciowych odczytywane jako s�owa.
vpunpcklbw ymm4, ymm3, ymm0 ;ymm4 - m�odsze bajty s�siad�w z lewej odczytywane jako s�owa.
vpunpckhbw ymm3, ymm3, ymm0 ;ymm3 - starsze bajty s�siad�w z lewej odczytywane jako s�owa.
vpunpcklbw ymm6, ymm5, ymm0 ;ymm6 - m�odsze bajty s�siad�w z prawej odczytywane jako s�owa.
vpunpckhbw ymm5, ymm5, ymm0 ;ymm5 - starsze bajty s�siad�w z prawej odczytywane jako s�owa.
vpunpcklbw ymm8, ymm7, ymm0 ;ymm8 - m�odsze bajty s�siad�w z lewego dolnego rogu odczytywane jako s�owa.
vpunpckhbw ymm7, ymm7, ymm0 ;ymm7 - starsze bajty s�siad�w z lewego dolnego rogu odczytywane jako s�owa.
vpunpcklbw ymm10, ymm9, ymm0 ;ymm10 - m�odsze bajty s�siad�w z prawego dolnego rogu odczytywane jako s�owa.
vpunpckhbw ymm9, ymm9, ymm0 ;ymm9 - starsze bajty s�siad�w z prawego dolnego rogu odczytywane jako s�owa.
vpunpcklbw ymm12, ymm11, ymm0 ;ymm12 - m�odsze bajty s�siad�w z lewego g�rnego rogu odczytywane jako s�owa.
vpunpckhbw ymm11, ymm11, ymm0 ;ymm11 - starsze bajty s�siad�w z lewego g�rnego rogu odczytywane jako s�owa.
vpunpcklbw ymm14, ymm13, ymm0 ;ymm14 - m�odsze bajty s�siad�w z prawego g�rnego rogu odczytywane jako s�owa.
vpunpckhbw ymm13, ymm13, ymm0 ;ymm13 - starsze bajty s�siad�w z prawego g�rnego rogu odczytywane jako s�owa.

vpaddw ymm1, ymm1,  ymm3 ;dodanie warto�ci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm4 
vpsubw ymm1, ymm1,  ymm5 ;odejmowanie warto�ci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm6 
vpaddw ymm1, ymm1,  ymm7 ;dodanie warto�ci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm8 
vpsubw ymm1, ymm1,  ymm9 ;odejmowanie warto�ci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm10 
vpaddw ymm1, ymm1,  ymm11 ;dodanie warto�ci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm12 
vpsubw ymm1, ymm1,  ymm13 ;odejmowanie warto�ci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm14 

vpackuswb ymm2, ymm2, ymm1 ;ponowne spakowanie s��w do bajt�w. Z nasyceniem bez znaku.
vmovdqu YMMWORD PTR[rdx], ymm2 ;zapisanie wyniku w tablicy wynikowej.

add r11,32 ;zwi�kszenie licznika o liczb� przetworzonych bajt�w.
add rcx,32 ;przesuni�cie wskaznika tablicy wej�ciowej za przetworzone bajty.
add rdx,32 ;przesuni�cie wskaznika tablicy wyj�ciowej za zapisane bajty.
cmp r11,r8 ;warunek ko�ca p�tli. licznik < dataSize
jl przetwarzanie_wektora
ret
filter endp

END