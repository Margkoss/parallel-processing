#include <stdio.h>
#include <stdlib.h>

// Linked list definition
typedef struct node
{
    int data;
    struct node *next;
} node_t;

// empty linked list queue
node_t *create()
{
    return NULL;
}

void enqueue(int data, node_t **head)
{
    if (!(*head))
    {
        (*head) = (struct node *)malloc(1 * sizeof(struct node));
        (*head)->next = NULL;
        (*head)->data = data;
    }
    else
    {
        node_t *current = (*head);
        while (current->next != NULL)
        {
            current = current->next;
        }

        /* now we can add a new variable */
        current->next = (node_t *)malloc(sizeof(node_t));
        current->next->data = data;
        current->next->next = NULL;
    }
}

// Debugging function that traverses linked list and prints elements
void print_list(node_t *head)
{
    if (!head)
    {
        printf("No items yet\n");
    }
    else
    {

        node_t *current = head;

        while (current != NULL)
        {
            printf("%d \n", current->data);
            current = current->next;
        }
        printf("\n");
    }
}

int dequeue(node_t **head)
{
    if (!(*head))
    {
        return -1;
    }
    else
    {
        node_t *temp;
        int toPop = (*head)->data;

        temp = (*head);
        (*head) = (*head)->next;
        temp->next = NULL;

        free(temp);
        return toPop;
    }
}

int isEmpty(node_t *head)
{
    return head == NULL;
}