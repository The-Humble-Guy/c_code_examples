#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

typedef enum {
    L_EXP_BEGIN,
    L_EXP_END,
    L_BRACKET_BEGIN,
    L_BRACKET_END,
    L_VARIABLE,
    L_ADD,
    L_SUB,
    L_MUL,
    L_DIV,
    L_POW,
    L_FUNCTION,
} lexem_t;

typedef enum {
    P_START_END,
    P_VARIABLE,
    P_ADD_SUB,
    P_MUL_DIV_POW,
    P_FUNCTION_UNARY,
    P_BRACKET,
} priority_t;

typedef struct {
    lexem_t lexem;
    priority_t priority;
    char value[50];
} token_t;

struct node_t {
    token_t token;
    struct node_t *next;
};

struct stack_elem {
    token_t token;
    struct stack_elem *prev;
};

typedef struct stack_elem custom_stack_t;

typedef struct node_t token_node_t;

#endif  // SRC_DEFINITIONS_H_
