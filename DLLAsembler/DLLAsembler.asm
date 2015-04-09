
__UNICODE__ equ 1

;include c:\masm32\include\masm32rt.inc
.486 
.model flat, stdcall 
.xmm

comparesse PROTO givenword :PTR WORD, wordtofind:PTR WORD

option casemap: none
.const
;--------------------------------
;argumenty dla rozkazow sse
FIND_EQUAL_CHARACTERS_U EQU  1001b 
MAKE_MASK_U equ 1101b
;-----------------------------------
space EQU  20h
check equ 0ffh

.code
;-------------------------------------------------------------------
; Funkcja cmpstr w wersji asemblerowej z wykorzystanie sse z rozkazami na lancuchach znakow
; parametry wywolania:	word - slowo do przetworzenia
;						wordtofind - slowo szukane
;						len - dlugosc slowa
;
;
;-------------------------------------------------------------------
comparesse proc givenword: PTR WORD, wordtofind: PTR WORD
		pushad		
		mov edx, wordtofind								;przeniesienie wskaznika do rejestru	
		mov ebx, 20h									;znak spacji do rejsetru
		MovDqU xmm0, [edx]								;przeniesienie stringu do rejseru xmm0
		MOVD xmm1, ebx									;przeniesienie do xmm1 znaku spacji
		PcmpIstrM xmm1, xmm0, MAKE_MASK_U				;wyliczanie maski
		movq xmm7, xmm0									;przeniesienie maski do rejestru 
		
		mov ebx, givenword								;przeniesienie wskaznika do rejestru	
		MovDqU    xmm0, [ebx] 							;Przeniesienie stringow do rejestrow xmm
		MovDqU    xmm1, [edx] 
		PcmpIstrM xmm0, xmm1, FIND_EQUAL_CHARACTERS_U	;znajdz identyczne znaki
		movd eax,xmm0									;przeniesienie wynikow do rejsetrow
		movd ebx,xmm7
		xor eax, ebx									;porownaj wynik z wyliczona maska
		mov bx, check									;sprawdzenie czy po xorwaniu mamy zerowy wynik
		sub ax, bx
		jz equal										;jesli zero to znaczy slowo pasuje do klucza
		jmp diff										;slowo nie pasuje do klucza
diff:   popad												
		xor eax, eax									;zwracanie adresu 0 czyli NULL
		ret  
equal:	popad
		mov eax, word									;przenies do akumulatora wskaznik na slowo sprawdzane
		ret
comparesse endp

end 


