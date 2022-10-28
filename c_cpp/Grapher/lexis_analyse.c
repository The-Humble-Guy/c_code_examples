#include "lexis_analyse.h"

char *erase_spaces(const char *input_string) {
    char *new_string = NULL;
    if (input_string) {
        new_string = calloc(strlen(input_string) + 1, sizeof(char));
        int pos = 0;
        for (int i = 0; input_string[i] != '\n'; ++i) {
            if (input_string[i] != ' ') {
                new_string[pos] = input_string[i];
                ++pos;
            }
        }
        new_string[pos] = '\0';
    }
    return new_string;
}

int is_staff(char symbol) {
    int flag = 0;
    switch (symbol) {
        case '+':
        case '^':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
            flag = 1;
            break;
        default:
            break;
    }
    return flag;
}

token_t *get_lexem(char *str) {
    token_t *token = NULL;
    if (str[0] != '\0') {
        token = calloc(1, sizeof(token_t));

        char string[10] = {0};
        size_t pos = 0;
        string[pos] = str[pos];

        if (!is_staff(string[pos])) {
            while (!is_staff(str[pos + 1]) && pos + 1 < strlen(str)) {
                string[pos + 1] = str[pos + 1];
                ++pos;
            }
        }
        string[pos + 1] = '\0';
        switch (string[0]) {
            case '+':
                token->lexem = L_ADD;
                token->priority = P_ADD_SUB;
                break;
            case '-':
                token->lexem = L_SUB;
                token->priority = P_ADD_SUB;
                break;
            case '*':
                token->lexem = L_MUL;
                token->priority = P_MUL_DIV_POW;
                break;
            case '/':
                token->lexem = L_DIV;
                token->priority = P_MUL_DIV_POW;
                break;
            case '^':
                token->lexem = L_POW;
                token->priority = P_MUL_DIV_POW;
                break;
            case '(':
                token->lexem = L_BRACKET_BEGIN;
                token->priority = P_BRACKET;
                break;
            case ')':
                token->lexem = L_BRACKET_END;
                token->priority = P_BRACKET;
                break;
            default:
                token->lexem = L_VARIABLE;
                token->priority = P_VARIABLE;
        }
        strcpy(token->value, string);
    }
    return token;
}

token_node_t *list_lexems(char *input) {
    token_t *token = NULL;

    token_t first = {L_EXP_BEGIN, P_START_END, ""};
    token_node_t *list = init(first);
    size_t pos = 0;
    while ((token = get_lexem(input + pos))) {
        push_back(list, *token);
        pos += strlen(token->value);
        free(token);
    }
    token_t end = {L_EXP_END, P_START_END, ""};
    push_back(list, end);
    correct_lexems(list);
    correct_unary(list);
    return list;
}

void correct_variable(token_t *token) {
    char *arr[] = {"sin", "cos", "tan", "ctg", "sqrt", "ln"};
    for (int i = 0; i < 6; ++i) {
        if (strcmp(token->value, arr[i]) == 0) {
            token->lexem = L_FUNCTION;
            token->priority = P_FUNCTION_UNARY;
        }
    }
}

void correct_lexems(token_node_t *list) {
    while (list) {
        token_node_t *next = list->next;
        if (list->token.lexem == L_VARIABLE) correct_variable(&list->token);
        list = next;
    }
}

int check_lexems(token_node_t *list) {
    int flag = 1;
    char *end;
    int var_load = 0;
    char var_name[10] = {};
    while (list && flag) {
        token_node_t *next = list->next;
        if (list->token.lexem == L_VARIABLE) {
            if (strtod(list->token.value, &end) == 0) {
                if (!var_load) {
                    strcpy(var_name, list->token.value);
                    var_load = 1;
                } else {
                    if (strcmp(var_name, list->token.value) != 0) flag = 0;
                }
            }
        }
        list = next;
    }
    return flag;
}

void correct_unary(token_node_t *list) {
    token_node_t *prev = NULL;
    token_node_t *cur = list;
    while (cur) {
        if (cur->token.lexem == L_SUB || cur->token.lexem == L_ADD) {
            if (!prev || (prev->token.lexem == L_BRACKET_END) || (prev->token.lexem > L_VARIABLE) ||
                (prev->token.lexem < L_VARIABLE)) {
                cur->token.priority = P_FUNCTION_UNARY;
            }
        }
        prev = cur;
        cur = cur->next;
    }
}
