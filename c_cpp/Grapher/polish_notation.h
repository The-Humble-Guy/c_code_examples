#ifndef SRC_POLISH_NOTATION_H_
#define SRC_POLISH_NOTATION_H_

#include "lexis_analyse.h"
#include "stack.h"
#include "list.h"
#include <math.h>
#include <stdio.h>


token_node_t *to_polish(token_node_t* tokens);

double evaluate(token_node_t *list, double x);

double operate(custom_stack_t **stack, token_t op, double x);

#endif  // SRC_POLISH_NOTATION_H_
