#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#include <stdlib.h>
#include <string.h>
#include "definitions.h"

token_node_t* init(token_t token);

token_node_t* push_back(token_node_t* root, token_t token);

void destroy(token_node_t* root);

#endif  // SRC_LIST_H_
