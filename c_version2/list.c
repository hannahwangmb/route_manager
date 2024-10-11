/** @file list.c
 *  @brief Implementation of a linked list.
 *
 * Based on the implementation approach described in "The Practice
 * of Programming" by Kernighan and Pike (Addison-Wesley, 1999).
 *
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "list.h"

/* a function to add a new node to the end of the list */
list_t *add_end(list_t *list, list_t *new)
{
    list_t *curr;

    if (list == NULL)
    {
        new->next = NULL;
        return new;
    }

    for (curr = list; curr->next != NULL; curr = curr->next)
        ;
    curr->next = new;
    new->next = NULL;
    return list;
}

/* a function to sort the list */
list_t *sort_list(list_t *list, int question) {
    list_t *sorted_list = NULL;
    list_t *current, *prev, *max_node, *max_node_prev;

    while (list != NULL) {
        max_node = list;
        max_node_prev = NULL;
        prev = list;
        current = list->next;
        while (current != NULL) {
            if (question == 1 || question == 3){
            if (atoi(current->statistic) < atoi(max_node->statistic)) {
                max_node = current;
                max_node_prev = prev;
            }
            else if (atoi(current->statistic) == atoi(max_node->statistic)) {
                if (strcmp(current->subject, max_node->subject) > 0) {
                    max_node = current;
                    max_node_prev = prev;
                }
            }
            }
        
            else if (question == 2){
                if (atoi(current->statistic) > atoi(max_node->statistic)) {
                max_node = current;
                max_node_prev = prev;
                }
                else if (atoi(current->statistic) == atoi(max_node->statistic)) {
                    if (strcmp(current->subject, max_node->subject) > 0) {
                        max_node = current;
                        max_node_prev = prev;
                    }
                }
            }
            
            prev = current;
            current = current->next;
        }

        if (max_node_prev == NULL) {
            list = list->next;
        } else {
            max_node_prev->next = max_node->next;
        }

        max_node->next = sorted_list;
        sorted_list = max_node;
    }

    return sorted_list;
}

