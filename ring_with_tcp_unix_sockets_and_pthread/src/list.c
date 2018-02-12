/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 12-02-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 12-02-2018
 */

/*
 * File: main.c
 * --------------------
 *  In this file is all the source code
 *  for the functions which handels the list.
 */

#include "ring.h"

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 */
List *new_list(void)
{
	List *newList = (List*)malloc(sizeof(List));
	*newList = (List){NULL,0};
	return newList;
}

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 */
struct Node *new_node(void *data)
{
	struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
	*newNode = (struct Node){NULL,data};
	return newNode;
}

/*
 * Function: add_first
 * Usage: Adds a new node first in the list.
 * ------------------------------------------
 */
void add_first(List *l,void *data)
{
	struct Node *newNode = new_node(data);
	newNode->next = l->first;
	l->first = newNode;
	l->size++;
}

/*
 * Function: remove_first
 * Usage: Removes the first node in the list.
 * -------------------------------------------
 */
void remove_first(List *l)
{
	if (l->first != NULL)
	{
		struct Node *removeNode = l->first;
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
void *get_first(List *l)
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
int list_size(List *l)
{
	return l->size;
}
