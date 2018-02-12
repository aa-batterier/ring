#include "ring.h"

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 */
list_S *new_list(void)
{
	list_S *newList = (list_S*)malloc(sizeof(list_S));
	*newList = (list_S){NULL,0};
	return newList;
}

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 */
struct node_S *new_node(void *data)
{
	struct node_S *newNode = (struct node_S*)malloc(sizeof(struct node_S));
	*newNode = (struct node_S){NULL,data};
	return newNode;
}

/*
 * Function: add_first
 * Usage: Addes a new node to the front of the list.
 * --------------------------------------------------
 */
void add_first(list_S *l,void *data)
{
	struct node_S *newNode = new_node(data);
	newNode->next = l->first;
	l->first = newNode;
	l->size++;
}

/*
 * Function: remove_first
 * Usage: Removes the first node in the list.
 * -------------------------------------------
 */
void remove_first(list_S *l)
{
	if (l->first != NULL)
	{
		struct node_S *removeNode = l->first;
		l->first = removeNode->next;
		free(removeNode);
		l->size--;
	}
}

/*
 * Function: get_first
 * Usage: Returns the data from the first node.
 * ---------------------------------------------
 */
void *get_first(list_S *l)
{
	if (l->first != NULL)
	{
		return l->first->data;
	}
	return NULL;
}

/*
 * Function: list_size
 * Usage: Returns the size of the list.
 * -------------------------------------
 */
int list_size(list_S *l)
{
	return l->size;
}
