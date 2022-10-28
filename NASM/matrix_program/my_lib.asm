%define arg(n) ebp+(4*n)+8
%define local(n) ebp-(4*n)

;push parameters into function in reverse order (push last param, ..., push
;first param)

%macro DwordToStr 3; DwordToStr value, string, len_string
    push dword 1
    push dword %3
    push dword %2
    push dword %1
    call NumToStr
    add esp, 12
%endmacro

%macro WordToStr 3; WordToStr value, string, len_string
    push eax;save previous EAX
    push dword 1
    push dword %3
    push dword %2
    mov ax, word %1;convert word to dword
    cwde
    push eax
    call NumToStr
    pop eax;restore previous EAX
    add esp, 12
%endmacro

%macro ByteToStr 3; ByteToStr value, string, len_string
    push eax
    push dword 1
    push dword %3
    push dword %2
    mov al, byte %1
    cbw
    cwde
    push eax
    call NumToStr
    pop eax
    add esp, 12
%endmacro

;push dword lenOutput


%macro ArrToStr 5
    push %5
    push %4
    push %3
    push %2
    push %1
    call AToStr
    add esp, 20
%endmacro


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

section .text

IToStr:
                ;USE CDECL CONVENSION
                push edi
                push ebp
                mov ebp, esp

                xor eax, eax
                xor edx, edx;result number
                xor ecx, ecx
                ;mov esi, input
                mov ebx, 10

.remove_spaces:
                lodsb
                cmp al, 32
                je .remove_spaces

                cmp al, '-'
                je .save_minus
                jmp .cycle
.save_minus:
                movsx ax, al
                cwde
                mov ecx, eax
                lodsb
.cycle:
                cmp al, 10
                je .prod
                cmp al, 32
                je .prod
            
                movsx ax, al
                cwde
                sub eax, 48
                js .error
                cmp eax, 9
                ja .error
                mov edi, eax;digit in edi
                mov eax, edx;current num
                imul ebx
                jo .error
                add eax, edi
                jc .error
                mov edx, eax
                lodsb
                jmp .cycle
.prod:
                cmp ecx, '-'
                je .negative
                jmp .end
.negative:
                neg edx
.end:       
                mov eax, edx
                mov ebx, 0
                jmp .fini
.error:
                mov eax, 0
                mov ebx, 1
.fini:

                mov esp, ebp
                pop ebp
                pop edi
                ret

AToStr:
                ;USE CDECL CONVENSION
                ;INIT
                push edi
                push ebp
                mov ebp, esp
                sub esp, 8
                ;params:
                ;   arg(1) - array address
                ;   arg(2) - count of numbers in string
                ;   arg(3) - dimensity of number (1, 2 or 4 bytes)
                ;   arg(4) - string address
                ;   arg(5) - lenght of string
                ;local variables:
                ;   local(1) - precision of single number

                mov eax, dword [arg(5)]
                dec eax
                add eax, [arg(4)]
                mov byte [eax], 10                                  ;type enter symbol at the end of string
                mov eax, dword [arg(5)]
                dec eax
                mov ecx, dword [arg(2)]                             ;ECX as counter for loop
                cdq
                idiv ecx                                            ;EAX contains precision of single number
                mov [local(1)], eax                                 ;use local(1) as precision of single number
                mov edx, [arg(4)]                                   ;EDX contain string address
                mov [local(2)], edx                                 ;local(2) for string adress

.cycle:
                dec ecx
                push ecx                                            ;NumToStr will rewrite ECX, so need to save current value of ECX

                push dword 0
                push dword [local(1)]                               ;push number of bytes for 1 integer

                mov eax, ecx                                        ;EAX = ECX
                imul dword [local(1)]                               ;Result in EAX:EDX; EAX = ECX*precision -start point
                add eax, [arg(4)]                                   ;EAX=ECX*precision + adrress of string

                push dword eax                                      ;push address to write number

                mov eax, ecx                                        ;number of element
                imul dword [arg(3)]                                 ;imul position number on displacement
                add eax, [arg(1)]                                   ;EAX contain the address of element in array

                cmp dword [arg(3)], 4                               ;find way to correctly push value of element
                je .case_4                                          ;is value dword?

                cmp dword [arg(3)], 2                               ;is value word?
                je .case_2

                cmp dword [arg(3)], 1                               ;is value byte?
                je .case_1
.case_1:
                movsx ax, byte [eax]
                cwde
                jmp .push_eax
.case_2:
                movsx eax, word [eax]
                jmp .push_eax
.case_4:
                mov eax, dword [eax]
                jmp .push_eax
.push_eax:
                push dword eax                                      ;push value of element of array
                call NumToStr
                add esp, 16

                pop ecx                                             ;recover correct number of iteration
                
                cmp ecx, 0                                          ;while non zero
                jne .cycle

                ;FINI
                mov esp, ebp
                pop ebp
                pop edi
                ret


NumToStr:
                ;USE CDECL convension
                ;INIT
                push edi
                push ebp
                mov ebp, esp
                sub esp, 16
                
                ;params:
                ;   arg(1) - number to convert
                ;   arg(2) - address of string
                ;   arg(3) - length of string
                ;   arg(4) - need to add '10' symbol at the end (bool)
                ;local variables:
                ;   local(1) - used as sign flag of number
                ;   local(2) - base of number system
                mov eax, dword [arg(1)]                     ;put integer into register
                mov dword [local(1)], 0                     ;use for pos(0) or neg(1) number (in flag purpose)
                mov dword [local(2)], 10                    ;base of number system
                mov edi, [arg(2)]                           ;save output string address
                mov ecx, 0
                cmp eax, 0                                  ;if EAX<0 need to save '-' and after negative EAX
                jl .minus
                jmp .continue
.minus:
                mov dword [local(1)], 1                     ;save that EAX is negative
                neg eax
.continue:
                add ecx, dword [arg(3)]                     ;number of digit for output including enter-character
                cmp dword [arg(4)], 1
                je .add_enter
                jmp .define_digits
.add_enter:
                dec ecx
                mov byte [edi+ecx], 10                      ;enter-byte at the end of string
.define_digits:
                dec ecx
                cdq
                idiv dword [local(2)]                       ;result in AX:DX, so digit in DX
                add dl, 48                                  ;convert digit to ASCII-code
                mov byte [edi+ecx], dl                      ;write digit 
                cdq
                cmp eax, 0                                  ;while EAX != 0 define digits
                jne .define_digits
    
                cmp dword [local(1)], 1                     ;EAX is negative ?
                je .add_minus
                cmp ecx, 0
                jne .spaces
                jmp .fini
.add_minus:
                dec ecx
                mov byte [edi+ecx], "-"                     ;write '-' before number
                cmp ecx, 0
                jne .spaces
                jmp .fini
.spaces:
                dec ecx
                mov byte [edi+ecx], 32                      ;add spaces before number
                cmp ecx, 0
                jne .spaces
                jmp .fini
                ;FINI
.fini:
                mov esp, ebp
                pop ebp
                pop edi
                ret


