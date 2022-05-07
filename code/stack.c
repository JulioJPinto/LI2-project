#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "conversions.h"

/**
 * @brief Create a stack object
 * @param initial_capacity capacidade inicial da stack
 * @return Pointer para a stack criada
 */
Stack *create_stack(int initial_capacity) {
    Stack *stack = malloc(sizeof(Stack));

    stack->capacity = initial_capacity;
    stack->current_index = -1;
    stack->array = calloc((unsigned long) initial_capacity, sizeof(StackElement));

    return stack;
}

/**
 * @brief Dá free à memória alocada pela stack
 * @param stack target
 */
void free_stack(Stack *stack) {
    for (int i = 0; i < length(stack); ++i) {
        free_element(stack->array[i]);
    }

    free(stack->array);
    free(stack);
}

/**
 * @brief  
 * @param element 
 */
void dump_element(StackElement *element) {
    switch ((*element).type) {
        case LONG_TYPE:
            printf("%ld", element->content.long_value);
            return;
        case CHAR_TYPE:
            printf("%c", element->content.char_value);
            return;
        case DOUBLE_TYPE:
            printf("%g", element->content.double_value);
            return;
        case STRING_TYPE:
            printf("%s", element->content.string_value);
            return;
        case ARRAY_TYPE:
            dump_stack(element->content.array_value);
            return;
        default: PANIC("Couldn't match type for %d when dumping\n", (*element).content.char_value)
    }
}

/**
 * @brief Imprime todos os valores da stack
 * @param stack target
 */
void dump_stack(Stack *stack) {
    for (int i = 0; i < length(stack); ++i) {
        dump_element(&stack->array[i]);
    }
}

/**
 * @brief Devolve o tamanho da stack.
 * @param stack target
 * @return tamanho da stack
 */
int length(Stack *stack) {
    return stack->current_index + 1;
}

/**
 * @brief Retira o elemento que se encontra no topo da stack
 * @param stack target
 * @return StackElement 
 */
StackElement pop(Stack *stack) {
    if(length(stack) <= 0) PANIC("Trying to pop from empty stack")

    StackElement result = stack->array[stack->current_index];
    stack->current_index--;

    return result;
}

/**
 * @brief Retira o elemento que se encontra no topo da stack e acessa o valor long dele
 * @param stack target
 * @return valor long acessado
 */
long pop_long(Stack *stack) {
    StackElement element = pop(stack);
    long value = element.content.long_value;

    free_element(element);
    return value;
}

/**
 * @brief Põem no topo da stack o elemento da x
 * @param stack target
 * @param x O elemento que vamos inserir no topo da stack
 */
void push(Stack *stack, StackElement x) {
    if (length(stack) >= stack->capacity) {
        stack->capacity *= 2;
        stack->array = realloc(stack->array, (unsigned long) stack->capacity * sizeof(StackElement));
        PRINT_DEBUG("REALLOCATED STACK (new capacity = %d)\n", stack->capacity)
    }

    stack->array[++(stack->current_index)] = x;
}

/**
 * @brief Push para tipos double
 */
void push_double(Stack *stack, double value) {
    push(stack, create_double_element(value));
}

/**
 * @brief Push para tipos long
 */
void push_long(Stack *stack, long value) {
    push(stack, create_long_element(value));
}

/**
 * @brief Push para tipos char
 */
void push_char(Stack *stack, char value) {
    push(stack, create_char_element(value));
}

/**
 * @brief Push para tipos string
 */
void push_string(Stack *stack, char *value) {
    push(stack, create_string_element(value));
}

void push_array(Stack *stack, Stack *value) {
    push(stack, create_array_element(value));
}

/**
 * @brief Cria um elemento do tipo double
 * @param value o valor do elemento
 * @return StackElement criado
 */
StackElement create_double_element(double value) {
    StackElement element;
    element.type = DOUBLE_TYPE;
    element.content.double_value = value;

    return element;
}

/**
 * @brief Cria um elemento do tipo long
 * @param value o valor do elemento
 * @return StackElement criado
 */
StackElement create_long_element(long value) {
    StackElement element;
    element.type = LONG_TYPE;
    element.content.long_value = value;

    return element;
}

/**
 * @brief Cria um elemento do tipo cbar
 * @param value o valor do elemento
 * @return StackElement criado
 */
StackElement create_char_element(char value) {
    StackElement element;
    element.type = CHAR_TYPE;
    element.content.char_value = value;

    return element;
}

/**
 * @brief Cria um elemento do tipo string
 * @param value o valor do elemento
 * @return StackElement criado
 */
StackElement create_string_element(char *value) {
    StackElement element;
    element.type = STRING_TYPE;

    size_t length = strlen(value) + 1;
    char *copied_string = calloc(length, sizeof(char));
    strcpy(copied_string, value);

    element.content.string_value = copied_string;

    return element;
}

StackElement create_array_element(Stack *value) {
    StackElement element;
    element.type = ARRAY_TYPE;
    element.content.array_value = value;

    return element;
}

StackElement peek(Stack *stack) {
    return stack->array[stack->current_index];
}

StackElement get(Stack *stack, long index) {
    return stack->array[stack->current_index - index];
}

/**
 * @brief Função que testa se um elemento pertence ao grupo truthy ou não
 * @param a Elemento que vamos testar
 * @return int Valor que vamos retornar caso este pertenca ao grupo truthy ou não (1 ou 0)
 */
int is_truthy(StackElement *a) {
    switch (a->type) {
        case LONG_TYPE:
            return a->content.long_value != 0;
        case CHAR_TYPE:
            return a->content.char_value != '\0';
        case STRING_TYPE:
            return strlen(a->content.string_value) != 0;
        case DOUBLE_TYPE:
            return a->content.double_value != .0;
        case ARRAY_TYPE:
            return length(a->content.array_value) != 0;
        default: PANIC("Couldn't retrieve truthy value from type %d\n", a->type)
    }
}

void free_element(StackElement element) {
    switch (element.type) {
        case STRING_TYPE:
            free(element.content.string_value);
            return;
        case ARRAY_TYPE:
            free_stack(element.content.array_value);
            return;
        case LONG_TYPE:
        case CHAR_TYPE:
        case DOUBLE_TYPE:
        default:
            return;
    }
}

StackElement duplicate_array(StackElement element) {
    Stack *old_array = element.content.array_value;
    Stack *new_array = create_stack(old_array->capacity);
    for(int i = 0; i < length(old_array); i++) {
        push(new_array, duplicate_element(old_array->array[i]));
    }
        return create_array_element(new_array);
}

StackElement duplicate_element(StackElement element) {
    switch (element.type) {
        case STRING_TYPE:
            return create_string_element(element.content.string_value);
        case ARRAY_TYPE:
            return duplicate_array(element);
        case LONG_TYPE:
        case CHAR_TYPE:
        case DOUBLE_TYPE:
        default:
            return element;
    }
}
