#ifndef SRC_STACK_H_
#define SRC_STACK_H_

#include <stdlib.h>
#include <string.h>
#include "definitions.h"


custom_stack_t *init_stack(token_t token);

custom_stack_t *push(custom_stack_t *head, token_t token);

token_t *pop(custom_stack_t **elem);

void destroy_stack(custom_stack_t *top);

#endif  // SRC_STACK_H_
