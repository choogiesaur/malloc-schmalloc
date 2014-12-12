#include "sorted-list.h"
#include <stdlib.h>

SortedListPtr SLCreate(CompareFuncT cf) {
	// Creating and mallocing the sl to be returned via the pointer sl.
	SortedListPtr sl;
	sl = (struct SortedList *)malloc(sizeof(struct SortedList));
	// If malloc fails print an error statement and return NULL.
	if (sl == NULL) {
		//printf("Error: Out of memory.\n");
		return NULL;
	}
	// Set the compare and destroy functions to the ones SLCreate was given.
	sl->compareFunct = cf;
	return sl;
}

void SLDestroy(SortedListPtr list) {
	Node *temp;
	// Runs through the entire list, deleting the front node every time through.
	while (list->front != NULL) {
		temp = list->front;
		list->front = list->front->next;
		// Destroying the content pointed to by the node and then freeing the node.
		list->destroyFunct(temp->content);
		free(temp);
	}
	// Freeing the list after we destroy all the nodes.
	free(list);
}

void *SLFind(SortedListPtr list, void *newObj){
	Node *curr;
	curr = list->front;
	if (list->front == NULL) {
		return NULL;
	}
	// Searching through the list for the node.
	while (curr != NULL) {
		// If the node is found return it.
		if (list->compareFunct(curr->content, newObj) == 0) {
			return curr->content;
		} 
		else {
			curr = curr->next;
		}	
	}
	// Otherwise it doesn't exist.
	return NULL;
}

int SLInsert(SortedListPtr list, void *newObj) {
	Node *newNode;
	Node *curr;
	Node *prev;
	curr = list->front;
	prev = NULL;
	// Creating the node to be inserted.
	newNode = (Node *)malloc(sizeof(Node));
	// If malloc fails print an error statement and return 0.
	if (newNode == NULL) {
		//printf("Error: Out of memory.\n");
		return 0;
	}
	// Set the values of the new node.
	newNode->content = newObj;
	newNode->ptrCounter = 0;
	// If the list is empty just put the node in front.
	if (list->front == NULL) {
		list->front = newNode;
		newNode->ptrCounter++;
		newNode->next = NULL;
		return 1;
	}
	// Searching through the list for the correct position of the new node.
	while (curr != NULL) {
		// If a duplicate is found print an error statement and return 0.
		if (list->compareFunct(curr->content, newObj) == 0) {
			//printf("Error: Duplicate value insertion.\n");
			return 0;
		}
		/* 
		* If the current content is "smaller" than the content to be inserted, then
		* the content to be inserted must come right before the current content.
		*/
		else if (list->compareFunct(curr->content, newObj) < 0) {
			// Check to see if the new node is to be put in the front of the list.
			if (prev == NULL) {
				list->front = newNode;
				newNode->ptrCounter++;
				newNode->next = curr;
				return 1;
			}
			// Otherwise just insert the new node.
			else {
				prev->next = newNode;
				newNode->ptrCounter++;
				newNode->next = curr;
				return 1;
			}
		}
		/* 
		* If the current content is "bigger" than the content to be inserted, then
		* simply continue, fr the correct position must be somewhere after the
		* current node.
		*/ 
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	/*
	* If you get to the end of the non-empty list and all the content was larger,
	* then just throw the new node at the end (it is smaller than everything else
	* in the list.
	*/
	if (curr == NULL && prev != NULL) {
		prev->next = newNode;
		newNode->ptrCounter++;
		newNode->next = curr;
		return 1;
	}
	// Otherwise you somehow failed.
	return 0;
}

int SLRemove(SortedListPtr list, void *newObj) {
	Node *curr;
	Node *prev;
	curr = list->front;
	prev = NULL;
	// Search through the list for the node to be removed.
	while (curr != NULL) {
		// If found remove it from the list.
		if (list->compareFunct(curr->content, newObj) == 0) {
			// If it is in the front of the list.
			if (prev == NULL) {
				list->front = curr->next;
				curr->ptrCounter--;
				if (list->front != NULL) {
					list->front->ptrCounter++;
				}
			}
			else {
				prev->next = curr->next;
				curr->ptrCounter--;
				if (prev->next != NULL) {
					prev->next->ptrCounter++;
				}
			}
			// If its pointer counter falls to zero, delete it.
			if (curr->ptrCounter <= 0) {
				list->destroyFunct(curr->content);
				if (curr->next != NULL) {
					curr->next->ptrCounter--;
				}
				free(curr);
			}
			return 1;
		}
		/*
		* If the current content is larger than the content to be removed, move on.
		* The node to be removed must lie after this node.
		*/
		else if (list->compareFunct(curr->content, newObj) > 0) {
			prev = curr;
			curr = curr->next;
		}
		/*
		* If the current content is less than the content to be removed, simply
		* stop. The content to be removed doesn't exist in the list. Return 0.
		*/
		else {
			return 0;
		}
	}
	// Otherwise you couldn't find the element in the list.
	return 0;
}
