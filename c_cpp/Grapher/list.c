#include "list.h"

token_node_t* init(token_t token) {
    token_node_t *root = calloc(1, sizeof(token_node_t));
    root->next = NULL;
    root->token.priority = token.priority;
    root->token.lexem = token.lexem;
    strcpy(root->token.value, token.value);
    return root;
}

token_node_t* push_back(token_node_t* root, token_t token) {
    while (root->next) {
        root = root->next;
    }
    root->next = init(token);
    return root->next;
}

void destroy(token_node_t* root) {
    while (root) {
        token_node_t *next = root->next;
        free(root);
        root = next;
    }
}
