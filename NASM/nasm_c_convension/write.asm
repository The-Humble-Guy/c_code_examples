;//extern int write_at_end(char output[], const char input[], int start, int end, int limit = 255);
;int write_at_end(char output[],const char input[], int start, int end, int limit = 255) {
;    //find zero character position
;    int pos = 0;
;    for (int i = pos; i < limit; ++i) {
;        if (output[i] == '\0') {
;            pos = i;
;            break;
;        }
;    }
;    if (pos > 0) {
;        output[pos] = ' ';
;        ++pos;
;    }
;    //check correct space
;    if (pos + end - start + 1 >= limit) {
;        return -1;
;    }
;    record(output, input, start, pos, (end - start + 1));
;    return 0;
;}

extern _Z6recordPcPKciiii
global _Z12write_at_endPcPKciii

section .text

_Z12write_at_endPcPKciii:
                            ;ebp+8 = char output[]
                            ;ebp+12 = const char input[]
                            ;ebp+16 = int start
                            ;ebp+20 = int end
                            ;ebp+24 = int limit

                            ;INIT
                            push ebp
                            mov ebp, esp

                            ;MAIN
                            xor eax, eax
                            xor ecx, ecx
.look_for_brecket:
                            mov eax, [ebp+8]
                            add eax, ecx
                            mov al, [eax]
                            ;mov eax, [ebp+8+ecx] ;load symbol (char output[i])
                            cmp al, 0 ;if zero symbol => find place to write
                            je .go_check
                            inc ecx ;else inc ecx and check next symbol
                            cmp ecx, [ebp+24] ;compare ecx and limit
                            jng .look_for_brecket
.go_check:
                            cmp ecx, 0 ;is it start of string?
                            jg .write_space
                            jmp .check_correct
.write_space:
                            mov eax, [ebp+8]
                            add eax, ecx
                            mov byte[eax], 32
                            ;mov byte[ebp+12+ecx], 32 ;add space before second or
                            ;                                       greater word
                            inc ecx
.check_correct:
                            mov eax, ecx ;EAX contains pos
                            add eax, [ebp+20] ;check (pos + end - start + 1 >= limit)
                            sub eax, [ebp+16]
                            inc eax
                            cmp eax, [ebp+24]

                            jge .ret_error
                            jmp .call_record
.ret_error:
                            mov eax, -1
                            jmp .fini

                            mov eax, ecx ;eax contain pos
                            

.call_record:
;    record(output, input, start, pos, (end - start + 1));

                            mov eax, ecx ;eax contain pos
                            
                            mov ecx, 255
                            push ecx;push limit

                            mov ecx, [ebp+20];load end
                            mov edx, [ebp+16];load start
                            sub ecx, edx
                            inc ecx
                            push ecx;push (end-start+1)

                            push eax;push pos

                            mov ecx, [ebp+16]
                            push ecx;push start

                            mov ecx, [ebp+12]
                            push ecx;push input

                            mov ecx, [ebp+8]
                            push ecx;push output
                            

                            call _Z6recordPcPKciiii
                            add esp, 24

                            mov eax, 0

.fini:
                            ;FINI
                            mov esp, ebp
                            pop ebp
                            ret

