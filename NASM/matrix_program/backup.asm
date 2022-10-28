%include "my_lib.asm"
%define M       7
%define N       4
%define sizeof  2

section .data
helloMsg db "Input 7x4 matrix string by string: ", 10
lenHello equ $-helloMsg

enterMsg db "You input the next values: ", 10
lenEnter equ $-enterMsg

resultMsg db "Transpose matrix: ", 10
lenResult equ $-resultMsg


exitMsg db "Press Enter to Exit", 10
lenExit equ $-exitMsg


section .bss
input resb 115
lenInput equ $-input

output resb 70
lenOutput equ $-output

A resw M*N
B resw N*M


section .text
global _start
_start:
            PRINT helloMsg, lenHello
            ;INPUT MATRIX
            mov ecx, 0;ECX as row counter
.global_cycle:
            push ecx
            SCAN input, lenInput;input string
            mov esi, input;load adress of string in esi
            mov edi, 0;EDI as column counter
            pop ecx
.cycle:
            push ecx;save ECX
            call IToStr;result in EAX
            pop ecx;restore ECX
            push edi
            push eax
;calculate offset in array
            mov eax, N*sizeof;A[i,j]=A+j*sizeof_row+i*sizeof
            imul ecx
            cdq
            mov ebx, eax
            mov eax, sizeof
            imul edi
            cdq
            add eax, ebx
            mov edi, eax
            
            pop eax
            mov [A+edi], ax;storage value at the same position
            pop edi
            inc edi
            cmp edi, N
            jne .cycle;while EDI != N
            
            inc esi
            inc ecx
            cmp ecx, M
            jne .global_cycle;while ESI != M

            ;TRANSPOSE
            ;the same logic
            ;need to correctly save offset in array A and B
            mov ecx, 0;i
.transpose:
            mov edi, 0;j
.transpose_cycle:
            mov eax, N*sizeof
            imul edi
            cdq
            mov ebx, eax
            mov eax, sizeof
            imul ecx
            cdq
            add eax, ebx;Adress the element in array A
            add eax, A
            mov ax, word [eax]
            cwde;A element in eax
            push eax;push value

            mov eax, M*sizeof
            imul ecx
            cdq
            mov ebx, eax
            mov eax, sizeof
            imul edi
            cdq
            add eax, ebx
            add eax, B
            pop ebx;pop value
            mov word [eax], bx;and storage

            inc edi
            cmp edi, M
            jne .transpose_cycle
            
            inc ecx
            cmp ecx, N
            jne .transpose

            mov eax, M*sizeof
            imul ecx
            cdq
            mov ebx, eax

            ;VIEW INPUT ARRAY
            PRINT enterMsg, lenEnter
            mov ecx, 0
.enter_cycle:
            mov eax, sizeof
            imul ecx
            add eax, A;calculate offset
            push ecx
            ArrToStr eax, N, sizeof, input, lenInput
            PRINT input, lenInput

            pop ecx
            add ecx, N
            cmp ecx, N*M
            jne .enter_cycle

            ;VIEW TRANSPOSE MATRIX
            PRINT resultMsg, lenResult
            mov ecx, 0
.result_cycle:
            mov eax, sizeof
            imul ecx
            add eax, B;calculate offset
            push ecx
            ArrToStr eax, M, sizeof, output, lenOutput
            PRINT output, lenOutput

            pop ecx
            add ecx, M
            cmp ecx, N*M
            jne .result_cycle

            ;SYSCALL FOR EXIT
            mov eax, 1
            xor ebx, ebx
            int 80h
 
