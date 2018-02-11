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
	return *newList;
}

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 */
node_S *new_node(void *data)
{
	node_S *newNode = (node_S*)malloc(sizeof(node_S));
	*newNode = (node_S){NULL,data};
	return *newNode;
}

/*
 * Function: add_first
 * Usage: Adds a new node to the front of the list.
 * -------------------------------------------------
 */
void add_first(list_S *l,void *data)
{
	node_S *newNode = new_node(data);
