#include "stdio.h"

int find_brecket(const char string[], int pos = 0) {
    for (int i = pos; string[i] != '\0'; ++i) {
        if ((string[i] == ' ') || (string[i] == '\n')) {
            return i;
        }
    }
    return -1;
}

bool contain_in_previous(const char string[], int start, int length) {
    for (int i = 0; i < start; ++i) {
        bool flag = true;
        if (string[i] == string[start] && string[i + length] == ' ') {
            for (int j = 0; j < length; ++j) {
                if (string[i + j] != string[start + j]) {
                    flag = false;
                }
            }
            if (((string[i - 1] == ' ') || (i == 0)) && flag) {
                return flag;
            }
        }
    }
    return false;
}

 void record(char out[],const char inp[], int start_in, int start_out, int lenght, int limit = 255) {
    for (int i = 0; i < lenght; ++i) {
        out[start_out + i] = inp[start_in + i];
    }
    out[start_out + lenght] = '\0';
    printf("Successfully copy word!\n");
}



//todo: Implement this C-code on Assembly
extern int write_at_end(char output[], const char input[], int start, int end, int limit = 255);
//int write_at_end(char output[],const char input[], int start, int end, int limit = 255) {
//    //find zero character position
//    int pos = 0;
//    for (int i = pos; i < limit; ++i) {
//        if (output[i] == '\0') {
//            pos = i;
//            break;
//        }
//    }
//    if (pos > 0) {
//        output[pos] = ' ';
//        ++pos;
//    }
//    //check correct space
//    if (pos + end - start + 1 >= limit) {
//        return -1;
//    }
//    record(output, input, start, pos, (end - start + 1));
//    return 0;
//}

int main(int argc, char **argv) {
    char input[255];
    char output[255];
    char dots[] = {'.', '.', '.'};
    output[0] = '\0';
    fgets(input, 255, stdin);
    int start = 0;
    int end = 0;
    while (find_brecket(input, start) != -1) {
        end = find_brecket(input, start) - 1;
        int length = end - start + 1;
        if (contain_in_previous(input, start, length)) {
            write_at_end(output, dots, 0, 2);
        } else {
            write_at_end(output, input, start, end);
        }
        start = end + 2;
    }
    printf("%s", output);
    return 0;
}
