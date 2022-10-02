#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    if (head == NULL) {
        return 0;
    }
    node *fast_ptr;
    node *slow_ptr;
    fast_ptr = head;
    slow_ptr = head;
    if (fast_ptr->next == NULL || fast_ptr->next->next == NULL) {
            return 0;
        }
    fast_ptr = fast_ptr->next->next;
    slow_ptr = slow_ptr->next;

    while (fast_ptr != NULL && fast_ptr != slow_ptr) {
        slow_ptr = slow_ptr->next;
        if (fast_ptr->next == NULL || fast_ptr->next->next == NULL) {
            return 0;
        }
        fast_ptr = fast_ptr->next->next;
    }
    return 1;
}
