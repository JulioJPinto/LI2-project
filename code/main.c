#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "operations.h"
#include "logica.h"
#include "conversions.h"
#include "logger.h"
#include "operations_storage.h"
#include "parser.h"

#define INPUT_BUFFER_SIZE 1000
#define INITIAL_STACK_CAPACITY 10

/**
 * @brief A função main, a função que conecta tudo na stack e a faz funcionar.
 */
int main() {
    char input[INPUT_BUFFER_SIZE];

    if (fgets(input, sizeof input, stdin) != input) {
        return EXIT_FAILURE;
    }

    Stack *stack = create_stack(INITIAL_STACK_CAPACITY);

    tokenize_and_parse(stack, input);

    dump_stack(stack);
    free_stack(stack);

    return 0;
}
