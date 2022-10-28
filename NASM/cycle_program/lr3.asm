%include "../lib.asm"

%macro PRINT 2
    mov eax, 4
    mov ebx, 1
    mov ecx, %1
    mov edx, %2
    int 80h
%endmacro

%macro SCAN 2
    mov eax, 3
    mov ebx, 0
    mov ecx, %1
    mov edx, %2
    int 80h
%endmacro

section .data
helloMsg db "Input values of 'q' and 'd': ", 10
lenHello equ $-helloMsg

resultMsg db "Result: ", 10
lenResult equ $-resultMsg

exitMsg db "Press Enter to Exit", 10
lenExit equ $-exitMsg

errorMsg db "Error! Zero division. Please enter new values", 10
lenError equ $-errorMsg

convErrorMsg db "Error! Uncorrect input. Please try again", 10
lenConvError equ $-convErrorMsg


section .bss
q resw 1
d resw 1
f resw 1
input resb 7
leninput equ $-input
output resb 10

section .text
global _start
_start:
            ;SHOW HELLO MESSAGE
            PRINT helloMsg, lenHello
            ;ENTER VALUES AND STORAGE
EnterQ:
            SCAN input, leninput
            mov esi, input
            call StrToInt
            cmp ebx, 0
            jne ErrorAtQ
            mov [q], ax
            jmp EnterD

ErrorAtQ:
            PRINT convErrorMsg, lenConvError
            jmp EnterQ

EnterD:
            SCAN input, leninput
            mov esi, input
            call StrToInt
            cmp ebx, 0
            jne ErrorAtD
            mov [d], ax
            jmp Calculate
ErrorAtD:
    
            PRINT convErrorMsg, lenConvError
            jmp EnterD

Calculate:
            ;CALCULATE
            cmp word [q], 10
            js else                                 ;if signed => reult q-10 < 0
            mov ax, [d]                             ;AX = d
            sub ax, [q]                             ;AX = d-q
            mov bx, 5                               ;BX = 5
            cwd
            imul bx                                 ;AX = AX * BX = (d - q) * 5
            jmp save_result

else:
            mov ax, [d]
            cmp ax, 5
            jz Error                                ;error if d - 5 == 0
            jmp Continue

Error:
            PRINT errorMsg, lenError
            jmp EnterQ

Continue:
            mov ax, [q]                             ;AX = q
            imul ax                                 ;AX = q * q
            cwd
            mov bx, [d]                             ;BX = d
            sub bx, 5                               ;BX = d - 5
            idiv bx                                 ;AX = (q * q) // (d -5)
            cwd
            add ax, [d]                             ;AX = (q * q) // (d - 5) + d

save_result:
            mov [f], ax
            ;VIEW RESULT
            PRINT resultMsg, lenResult
    
            mov esi, output
            mov ax, [f]
            cwde
            call IntToStr                           ; lenght in eax, start at [esi]
            PRINT esi, eax

            ;SHOW EXIT MESSAGE
            PRINT exitMsg, lenExit
            SCAN input, leninput

            ;syscall for exit
            mov eax, 1
            xor ebx, ebx
            int 80h
