#include "structures.h"
#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern char *exp_ptr;
extern int brackets_count;

Pointer *brackets_handler()
{
    Pointer *number = create_number(0, 0);
    Pointer *root = create_operator('+', NULL, number);
    Pointer *oprs[4] = {root, NULL, NULL, NULL};
    create_tree(oprs);
    if (*exp_ptr == ')')
    {
        exp_ptr++;
        brackets_count--;
    }
    if (oprs[0] == NULL) // If there was an error in creating the tree, we return NULL.
        return NULL;
    solve_tree(root);
    return root;
}

// Creating a tree ignoring the brackets inside.
// If there are brackets, we let the function brackets_handler handle it.
void create_tree(Pointer *oprs[4])
{
    // Reading the number:
    Pointer *number;
    if (*exp_ptr == ' ')
        exp_ptr++;
    else if (*exp_ptr == '(')
    {
        exp_ptr++;
        brackets_count++;
        number = brackets_handler();
    }
    else if (*exp_ptr == '-')
    {
        // Handling the case where the expressions starts with a negative number.
        // If the first character is a minus sign, we multiply the number by -1.
        number = create_number(-1, 0);
        *exp_ptr = '*';
    }
    else
        number = read_number();

    if (number == NULL)
    {
        // If there is no number, we raise the error flag.
        oprs[0] = NULL;
        return;
    }

    // Reading the operator:
    Pointer *operator;
    if (*exp_ptr == '\0' || *exp_ptr == ')')
    {
        oprs[0]->f.o.left = number;
        return;
    }
    if (*exp_ptr == 'i')
        operator = create_operator('*', NULL, NULL);
    else
        operator = read_operator();

    if (operator == NULL)
    {
        oprs[0] = NULL;
        return;
    }
    operator->f.o.left = number;

    create_tree(oprs); // We go deeper into the tree to read the next number & operator.
    if (oprs[0] == NULL)
        return; // If there was an error in creating the subtree, we return NULL.
    // Linking the current operator to the subtree.
    // Finding the right place for the operator in the subtree.
    // The operator is placed in the subtree according to its precedence.
    char pr = is_an_opr(operator->f.o.type), i;
    for (i = pr; i >= 0 && oprs[i] == NULL; i--)
        ;
    operator->f.o.right = oprs[i]->f.o.left;
    oprs[i]->f.o.left = operator;
    oprs[pr] = operator;
    for (i = pr + 1; i < 4; i++)
        oprs[i] = NULL;
}
