/*
 * File: PQueue.h
 * ---------------
 * Defines the interface for priority queue class by using a chunk list.
 * In the following document nodes represent index rather than value.
 */
#ifndef _pqueue_h
#define _pqueue_h


#include "genlib.h"
#include "disallowcopy.h"
#include "vector.h"
#include "cmpfn.h"

/* 
 * Class: PQueue
 * --------------
 * This is the class for a priority queue based on a chunklist.
 * This is not a FIFO queue, instead the dequeing process is done based upon the priority of the element.
 * The priority is saved as san integer. (Larger integer is higher priority)
 */
template <typename ElemType>
class PQueue{
	public:
		/* Constructor and destructor */
		PQueue(int (*comparator)(ElemType, ElemType) = OperatorCmp);
		~PQueue();

		/* Checks if the queue is empty or not*/
		bool isEmpty();

		/* Enqueues the new element to the priority queue*/
		void enqueue(ElemType newElement);

		/* Dequeues the element with highest priority */
		ElemType dequeueMax();

		/* Returns the size of queue*/
		int size();

		/* Returns the memory used by current queue. Not usually part of class, only for some performance
		 * evaluation purposes*/
		int bytesUsed();

		/*
		 * Member function: implementationName
		 * Usage: cout << pq.implementationName();
		 * ---------------------------------------
		 * This operation would not usually be included as part of the class
		 * class, but is included to help with analyzing and reporting results.
		 * This member function returns a string that describes the PQueue
		 * implementation strategy ("sorted linked list", "vector", etc.).
		 */
		string implementationName();

		/*
		 * Member function: printDebuggingInfo
		 * Usage: pq.printDebuggingInfo();
		 * -------------------------------
		 * This operation would not usually be included as part of the class,
		 * but is included to give you a hook to put any helpful debugging
		 * print code (for example, something that prints the goopy details of
		 * the internal structure). You don't need to implement this routine and can
		 * ignore it entirely, but you may find it helpful to use as you are
		 * doing development.
		 */
		void printDebuggingInfo();

	private:
		private:
		// If implemented using Vector data mamber, default memberwise copy 
		// works fine, but if implemented as linked list, (ie pointer data member)
		// copying would create unintended sharing.
		// It's fine is to disallow copying for all implementations as
		// a precaution
		DISALLOW_COPYING(PQueue)

		/* This is the representation of elements inside the priority queue for elements. The elements
		 * are stored inside a heap, which essentially is a flattened array of integers that dynamically grows
		 * as elements are added. The elements are indexed starting from 0. Each element's parent index can be 
		 * calculated by (childIndex - 1)/2 and ignoring the remainder. Conversely, the two child(left and right) of 
		 * each parent would respectively be at index: 2*parentIndex + 1 and 2*(parentIndex + 1)
		 */
		 Vector<ElemType> heap;  
		
		/* ===================== Enqueue helpers =====================*/
		/*
		 * Member function: bubbleUp
		 * -------------------------
		 * Helper to enqueue funtion to help the bubble up operation. This operation moves the newElement add at the
		 * last of heap to the appropriate position in heap. The behaviour is achieved by swapping the element with its parent
		 * till it is either less than or equal to its parent.
		 */
		void bubbleUp(int focusNode);
		/* Returns true if the passed node is root node of heap */
		bool PQueue::isRoot(int node);
		/* Returns the index of parent node of the node passed as argument */
		int PQueue::getParent(int node);
		/* Returns true if value at node1 is greater than value at node2. False otherwise. Assumes both node exist */
		bool PQueue::compareNode(int node1, int node2);
		/* Swaps the value at each node */
		void PQueue::swap(int node1, int node2);

        /* ============== Dequeue helpers ==================*/
		/* Function: isLast
		 * ----------------
		 * Returns true if the argument node is last in the heap
		 */
		bool isLast(int node);
		/* Function: heapify
		 * -----------------
		 * This function rearrange the heap in its proper heap form. Concretely, it rearrange the root node to its 
		 * apt position by comparing each parent with its child node and swapping till the parent is either the largest
		 * or at the last node.
		 */
		void heapify(int parentNode);
		/* Function: getChildren
		 * ---------------------
		 * This function saves the index of children of parent node in the children vector (passed by reference for efficiency reasons)
		 */
		void PQueue::getChild(int parentNode, Vector<int> &children);

		/* Pointer to callback function passed by client*/
		int (*comparator)(ElemType, ElemType);
};

#include "pqheap.cpp"


#endif