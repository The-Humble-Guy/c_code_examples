#include <stdio.h>
#include "lexis_analyse.h"
#include "polish_notation.h"
#include "draw.h"

int main() {
    char *buffer = NULL;
    size_t len = 0;
    getline(&buffer, &len, stdin);
    char *new = erase_spaces(buffer);
    token_node_t *list = list_lexems(new);
    if (check_lexems(list)) {
        token_node_t *polish = to_polish(list);
        draw_enter(polish);
        destroy(polish);
    } else {
        printf("Input function contain more than 1 variable. Please try again...");
    }
    destroy(list);

    free(new);
    free(buffer);
    return 0;
}
