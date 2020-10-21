;Temat: Filtrowanie uwypuklaj¹ce skierowane na zachód.
;Algorytm bêdzie filtrowa³ ka¿dy z kolorów pixeli zgodnie z mask¹
;1 0 -1
;1 1 -1
;1 0 -1
;Wykona³: Filip Gaida gr.3 semestr 5, 2020/2021
;Algortym w tym pliku by³ podstawowo napisany pod rejestry xmm, jednak dla testów rozszerzy³em go do rejestrów ymm
.code

filter proc ;RCX-pixels, RDX-newPixels, R8-dataSize, R9-w
xor r11,r11 ;wyzerowanie r11 s³u¿¹cego jako licznik.
vxorps ymm0, ymm0, ymm0 ; wyzerowanie ymm0 slu¿¹cego do konwersji spakowanych Byte na Word.
xor r10, r10 ;wyzerowanie r10 s³u¿¹cego jako przesuniêcie na bitmapie w dó³.
sub r10, r9 ;przypisanie do r10 ujemnej wartoœci szerokoœci.
przetwarzanie_wektora: 
vmovdqu ymm1, YMMWORD PTR[rcx] ;ymm1 - dane wejsciowe.
vmovdqu ymm7, YMMWORD PTR[rcx + r10 - 3] ;wektor s¹siadów z lewgo dolnego rogu.
vmovdqu ymm9, YMMWORD PTR[rcx + r10 + 3] ;wektor s¹siadów z prawego dolnego rogu.
vmovdqu ymm3, YMMWORD PTR[rcx - 3] ;wektor s¹siadów z lewej.
vmovdqu ymm5, YMMWORD PTR[rcx + 3] ;wektor s¹siadów z prawej.
vmovdqu ymm11, YMMWORD PTR[rcx + r9 - 3] ;wektor s¹siadów z lewgo górnego rogu.
vmovdqu ymm13, YMMWORD PTR[rcx + r9 + 3] ;wektor s¹siadów z lewgo górnego rogu.

vpunpcklbw ymm2, ymm1, ymm0 ;ymm2 - m³odsze bajty danych wejsciowych odczytywane jako s³owa.
vpunpckhbw ymm1, ymm1, ymm0 ;ymm1 - starsze bajty danych wejsciowych odczytywane jako s³owa.
vpunpcklbw ymm4, ymm3, ymm0 ;ymm4 - m³odsze bajty s¹siadów z lewej odczytywane jako s³owa.
vpunpckhbw ymm3, ymm3, ymm0 ;ymm3 - starsze bajty s¹siadów z lewej odczytywane jako s³owa.
vpunpcklbw ymm6, ymm5, ymm0 ;ymm6 - m³odsze bajty s¹siadów z prawej odczytywane jako s³owa.
vpunpckhbw ymm5, ymm5, ymm0 ;ymm5 - starsze bajty s¹siadów z prawej odczytywane jako s³owa.
vpunpcklbw ymm8, ymm7, ymm0 ;ymm8 - m³odsze bajty s¹siadów z lewego dolnego rogu odczytywane jako s³owa.
vpunpckhbw ymm7, ymm7, ymm0 ;ymm7 - starsze bajty s¹siadów z lewego dolnego rogu odczytywane jako s³owa.
vpunpcklbw ymm10, ymm9, ymm0 ;ymm10 - m³odsze bajty s¹siadów z prawego dolnego rogu odczytywane jako s³owa.
vpunpckhbw ymm9, ymm9, ymm0 ;ymm9 - starsze bajty s¹siadów z prawego dolnego rogu odczytywane jako s³owa.
vpunpcklbw ymm12, ymm11, ymm0 ;ymm12 - m³odsze bajty s¹siadów z lewego górnego rogu odczytywane jako s³owa.
vpunpckhbw ymm11, ymm11, ymm0 ;ymm11 - starsze bajty s¹siadów z lewego górnego rogu odczytywane jako s³owa.
vpunpcklbw ymm14, ymm13, ymm0 ;ymm14 - m³odsze bajty s¹siadów z prawego górnego rogu odczytywane jako s³owa.
vpunpckhbw ymm13, ymm13, ymm0 ;ymm13 - starsze bajty s¹siadów z prawego górnego rogu odczytywane jako s³owa.

vpaddw ymm1, ymm1,  ymm3 ;dodanie wartoœci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm4 
vpsubw ymm1, ymm1,  ymm5 ;odejmowanie wartoœci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm6 
vpaddw ymm1, ymm1,  ymm7 ;dodanie wartoœci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm8 
vpsubw ymm1, ymm1,  ymm9 ;odejmowanie wartoœci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm10 
vpaddw ymm1, ymm1,  ymm11 ;dodanie wartoœci zgodnie z zastosowanymi wagami
vpaddw ymm2, ymm2, ymm12 
vpsubw ymm1, ymm1,  ymm13 ;odejmowanie wartoœci zgodnie z zastosowanymi wagami
vpsubw ymm2, ymm2, ymm14 

vpackuswb ymm2, ymm2, ymm1 ;ponowne spakowanie s³ów do bajtów. Z nasyceniem bez znaku.
vmovdqu YMMWORD PTR[rdx], ymm2 ;zapisanie wyniku w tablicy wynikowej.

add r11,32 ;zwiêkszenie licznika o liczbê przetworzonych bajtów.
add rcx,32 ;przesuniêcie wskaznika tablicy wejœciowej za przetworzone bajty.
add rdx,32 ;przesuniêcie wskaznika tablicy wyjœciowej za zapisane bajty.
cmp r11,r8 ;warunek koñca pêtli. licznik < dataSize
jl przetwarzanie_wektora
ret
filter endp

END