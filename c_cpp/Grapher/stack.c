#include "stack.h"


custom_stack_t *init_stack(token_t token) {
    custom_stack_t *root = calloc(1, sizeof(custom_stack_t));
    root->token.priority = token.priority;
    root->token.lexem = token.lexem;
    strcpy(root->token.value, token.value);
    root->prev = NULL;
    return root;
}

custom_stack_t *push(custom_stack_t *head, token_t token) {
    custom_stack_t *new = init_stack(token);
    new->prev = head;
    return new;
}

token_t *pop(custom_stack_t **elem) {
    token_t *value = NULL;
    if (elem && *elem) {
        custom_stack_t *temp = *elem;
        value = &(*elem)->token;
        *elem = (*elem)->prev;
        free(temp);
    }
    return value;
}

void destroy_stack(custom_stack_t *top) {
    while (top) {
        custom_stack_t *cur = top->prev;
        free(top);
        top = cur;
    }
}
