#include "sorted-list.h"
#include <stdlib.h>


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

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

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
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


/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

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


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

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


/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
  SortedListIteratorPtr sli;
  // Creating the iterator.
  sli = (struct SortedListIterator *)malloc(sizeof(struct SortedListIterator));
  // If the malloc fails, print an error statement and return NULL.
  if (sli == NULL) {
    //printf("Error: Out of memory.\n");
    return NULL;
  }
  if (list->front == NULL) {
	return NULL;
  }
  // Set the sorted list and current fields to the proper values.
  sli->sortedList = list;
  sli->current = list->front;
  sli->current->ptrCounter++;
  return sli;
}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter) {
  Node *destroyer;
  Node *temp;
  // If iter is NULL don't do anything.
  if (iter == NULL) {
	  return;
  }
  // If iter is pointing to nothing simply free it.
  if (iter->current == NULL) {
	  free(iter);
  }
  // Otherwise there is a need to check to see if we need to destroy any nodes.
  else {
	  destroyer = iter->current;
	  // Destroy the iterator.
	  free(iter);
	  destroyer->ptrCounter--;
	  /*
	   * If the node the Iterator pointed to now has a counter of 0, we must destroy
	   * it and then assess whether or not we must now destory the next node. Destroying
	   * the iterator can have a cascading effect where many nodes that were removed
	   * are deleted.
	   */
	  while(destroyer->ptrCounter <= 0 && destroyer != NULL) {
		temp = destroyer;
		destroyer = destroyer->next;
		// Destroying the content of the node and the node itself.
		iter->sortedList->destroyFunct(temp->content);
		free(temp);
		// Decrementing the counter on the node after the destroyed node.
		destroyer->ptrCounter--;
	  }
  }
}


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
*/

void * SLGetItem(SortedListIteratorPtr iter) {
  // If the iterator advanced past the list return 0.
  if (iter->current == NULL) {
    return 0;
  }
  // Otherwise return the pointer to the content.
  else {
    return iter->current->content;
  }
}

/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void * SLNextItem(SortedListIteratorPtr iter) {
  Node *temp;
  // If the iterator advanced past the list return NULL.
  if (iter->current == NULL) {
    return NULL;
  }
  // Move the iterator and adjust the pointer counts on the involved nodes.
  temp = iter->current;
  iter->current = iter->current->next;
  if (iter->current != NULL) {
	  iter->current->ptrCounter++;
  }
  temp->ptrCounter--;
  // If the node the iterator left now falls to a pointer count of 0, delete it.
  if (temp->ptrCounter <= 0) {
    iter->sortedList->destroyFunct(temp->content);
    free(temp);
    /*
     * Decrement the current ptrCounter. No need to check if 0 because of 
     *iterator pointing to it.
     */
    if (iter->current != NULL) {
	  iter->current->ptrCounter--;
	}
  }
  // If the iterator advances past the list return NULL.
  if (iter->current == NULL) {
    return NULL;
  }
  // Otherwise return the pointer to the content.
  else {
    return iter->current->content;
  }
}
