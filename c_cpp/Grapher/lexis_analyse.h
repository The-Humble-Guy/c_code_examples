#ifndef SRC_LEXIS_ANALYSE_H_
#define SRC_LEXIS_ANALYSE_H_

#include "definitions.h"
#include "list.h"

char* erase_spaces(const char *input_string);
token_node_t *list_lexems(char *input);
token_t *get_lexem(char *str);
int is_staff(char symbol);
void correct_lexems(token_node_t *list);
void correct_variable(token_t *token);
int check_lexems(token_node_t *list);
void correct_unary(token_node_t *list);

#endif  // SRC_LEXIS_ANALYSE_H_
