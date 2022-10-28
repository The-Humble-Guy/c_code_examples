#include "polish_notation.h"

token_node_t *to_polish(token_node_t *tokens) {
    token_node_t *new_order = init(tokens->token);
    custom_stack_t *operations = NULL;
    tokens = tokens->next;
    token_t *current_token = &tokens->token;
    while (current_token->lexem != L_EXP_END) {
        if (current_token->lexem == L_VARIABLE) push_back(new_order, *current_token);

        if (current_token->lexem == L_FUNCTION || current_token->lexem == L_BRACKET_BEGIN)
            operations = push(operations, *current_token);

        if (current_token->lexem == L_BRACKET_END) {
            token_t *haystack = pop(&operations);
            while (haystack && haystack->lexem != L_BRACKET_BEGIN) {
                push_back(new_order, *haystack);
                haystack = pop(&operations);
            }
        }
        if (current_token->lexem == L_ADD || current_token->lexem == L_SUB || current_token->lexem == L_DIV ||
            current_token->lexem == L_MUL || current_token->lexem == L_POW) {
            while (operations && operations->token.priority >= current_token->priority &&
                   operations->token.priority != P_BRACKET) {
                push_back(new_order, *pop(&operations));
            }
            operations = push(operations, *current_token);
        }

        tokens = tokens->next;
        current_token = &tokens->token;
    }
    while (operations) {
        push_back(new_order, *pop(&operations));
    }
    return new_order;
}


double evaluate(token_node_t *list, double x) {
    double res = 0;
    char *end;
    token_t result = {L_VARIABLE, P_VARIABLE, ""};
    custom_stack_t *stack = init_stack(list->next->token);
    if (stack->token.lexem == L_VARIABLE) {
        if (strtod(stack->token.value, &end) == 0) {
            sprintf(stack->token.value, "%.10lf", x);
        }
    }
    list = list->next->next;
    token_t *cur = &list->token;
    while (cur) {
        double val = 99999;
        if ((val = operate(&stack, *cur, x)) != 99999) {
            res = val;
            sprintf(result.value, "%.25f", res);
            stack = push(stack, result);
        }
//        double val = operate(&stack, *cur, x);
//        res += val;
//        if (cur->lexem != L_VARIABLE) {
//            double val = operate(&stack, *cur, x);
//            sprintf(result.value, "%.25f", res);
//            stack = push(stack, result);
//        }
        list = list->next;
        cur = &list->token;
    }
    res = strtod(stack->token.value, &end);
    destroy_stack(stack);
    return res;
}

double operate(custom_stack_t **stack, token_t op, double x) {
    double result = 99999;
    lexem_t lexem = op.lexem;
    priority_t priority = op.priority;
    double var_1 = 0;
    double var_2 = 0;
    token_t token_1;
    token_t token_2;
    char *end;
    switch (lexem) {
        case L_VARIABLE:
            if (strtod(op.value, &end) == 0)
                sprintf(op.value, "%.25lf", x);
            (*stack) = push(*stack, op);
            break;
        case L_ADD:
            token_1 = *pop(stack);
            var_1 = strtod(token_1.value, &end);
            token_2 = *pop(stack);
            var_2 = strtod(token_2.value, &end);
            result = var_1 + var_2;
            break;
        case L_SUB:
            if (priority == P_FUNCTION_UNARY) {
                token_1 = *pop(stack);
                var_1 = strtod(token_1.value, &end);
                result = -var_1;
            } else {
                token_1 = *pop(stack);
                var_1 = strtod(token_1.value, &end);
                token_2 = *pop(stack);
                var_2 = strtod(token_2.value, &end);
                result = var_2 - var_1;
            }
            break;
        case L_MUL:
            token_1 = *pop(stack);
            var_1 = strtod(token_1.value, &end);
            token_2 = *pop(stack);
            var_2 = strtod(token_2.value, &end);
            result = var_1 * var_2;
            break;
        case L_DIV:
            token_1 = *pop(stack);
            var_1 = strtod(token_1.value, &end);
            token_2 = *pop(stack);
            var_2 = strtod(token_2.value, &end);
            if (var_1 > 0 || var_1 < 0) result = var_2 / var_1;
            break;
        case L_POW:
            token_1 = *pop(stack);
            var_1 = strtod(token_1.value, &end);
            token_2 = *pop(stack);
            var_2 = strtod(token_2.value, &end);
            if (var_1 < 0) {
                if (var_2 > 0 || var_2 < 0) result = pow(var_2, var_1);
            } else {
                result = pow(var_2, var_1);
            }
            break;
        case L_FUNCTION:
            token_1 = *pop(stack);
            var_1 = strtod(token_1.value, &end);
            if (strcmp(op.value, "sin") == 0) result = sin(var_1);
            if (strcmp(op.value, "cos") == 0) result = cos(var_1);
            if (strcmp(op.value, "tan") == 0) {
                if (cos(var_1)) result = sin(var_1) / cos(var_1);
            }
            if (strcmp(op.value, "ctg") == 0 && sin(var_1)) {
                if (sin(var_1)) result = cos(var_1) / sin(var_1);
            }
            if (strcmp(op.value, "sqrt") == 0 && var_1 >= 0) result = sqrt(var_1);
            if (strcmp(op.value, "ln") == 0 && var_1 >= 0) result = log(var_1);
        default:
            break;
    }
    return result;
}
