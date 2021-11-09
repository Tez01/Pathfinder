/* 
 * File: pqheap.cpp
 * ----------------
 * This file contains the implementation code of heap based priority queue.
 */
#include "pqueue.h"
#include "genlib.h"
#include <iostream>
#include "cmpfn.h"

/* Constructor */
template<typename ElemType>
PQueue<ElemType>::PQueue(int (*cmp)(ElemType, ElemType) = OperatorCmp){
	comparator = cmp;
}

/* Destructor */
template<typename ElemType>
PQueue<ElemType>::~PQueue(){
}

/* Checks if the queue is empty or not*/
template<typename ElemType>
bool PQueue<ElemType>::isEmpty(){
	return heap.isEmpty();
}


/* ========================= Enqueue level 1 decomposition ===================================== */
/* Returns true if the passed node is root node of heap */
template<typename ElemType>
bool PQueue<ElemType>::isRoot(int node){
	return node == 0;
}
/* Returns the index of parent node of the node passed as argument */
template<typename ElemType>
int PQueue<ElemType>::getParent(int node){
	return (node - 1)/2;
}
/* Returns true if value at node1 is greater than value at node2. False otherwise. Assumes both node exist */
template<typename ElemType>
bool PQueue<ElemType>::compareNode(int node1, int node2){
return comparator(heap.getAt(node1), heap.getAt(node2)) > 0;
}
/* Swaps the value at each node */
template<typename ElemType>
void PQueue<ElemType>::swap(int node1, int node2){
	ElemType temp = heap.getAt(node1);
	heap[node1] = heap[node2];
	heap[node2] = temp;
}

/*
 * Member function: bubbleUp
 * -------------------------
 * Helper to enqueue funtion to help the bubble up operation. This operation moves the newElement add at the
 * last of heap to the appropriate position in heap. The behaviour is achieved by swapping the element with its parent
 * till it is either less than or equal to its parent.
 */
template<typename ElemType>
void PQueue<ElemType>::bubbleUp(int focusNode){
	if(size() == 0)
		Error("Empty heap");
	if(isRoot(focusNode)) return;                    // Base case
	int parentNode = getParent(focusNode);
	if(!compareNode(focusNode, parentNode)) return;  // Base case

	swap(focusNode, parentNode);
	bubbleUp(parentNode);	                         // else
}



/* Enqueues new element to the priority queue*/
template<typename ElemType>
void PQueue<ElemType>::enqueue(ElemType newElement){
	heap.insertAt(size(), newElement);
	int focusNode = size() - 1;
	bubbleUp(focusNode);   // Moves the new element at appropriate position in the heap.
}

/* ================================= dequeue max decomposition level 1============================
/* Function: isLast
 * ----------------
 * Returns true if the argument node is last in the heap
 */
template<typename ElemType>
bool PQueue<ElemType>::isLast(int node){
	int child = 2*node + 1;
	return child > size() - 1 ;  // if child does not exist that means it is last
}

/* Function: getChildren
 * ---------------------
 * This function saves the index of children of parent node in the children vector (passed by reference for efficiency reasons)
 */
template<typename ElemType>
void PQueue<ElemType>::getChild(int parentNode, Vector<int> &children){
	int child1 = 2*parentNode + 1;     // left child
	int child2 = 2*(parentNode + 1);   // right child
	if(child1 <= (size() - 1))
		children.add(child1);    // add only if child exist
	if(child2 <= (size() - 1))  // add only if child exist
		children.add(child2);
}
/* Function: heapify
 * -----------------
 * This function rearrange the heap in its proper heap form. Concretely, it rearrange the root node to its 
 * apt position by comparing each parent with its child node and swapping till the parent is either the largest
 * or at the last node.
 */
template<typename ElemType>
void PQueue<ElemType>::heapify(int parentNode){
	if(isLast(parentNode)) return;
	Vector<int> children;   // Vector to store child indices.
	getChild(parentNode, children);
	if(children.size() > 1){   // if both child exist
		if(compareNode(parentNode, children[0]) && compareNode(parentNode, children[1])) return;
		
		else if(compareNode(children[0] , children[1])){  // if left children is greater, swap and recur
			swap(parentNode, children[0]);
			heapify(children[0]);
		}
		else{
			swap(parentNode, children[1]);          // else swap with right children and recur.
			heapify(children[1]);
		}
	}
	else{
		if(compareNode(children[0], parentNode)){
			swap(parentNode, children[0]);
			heapify(children[0]);
		}
	}
}

/* Dequeues the element with highest priority */
template<typename ElemType>
ElemType PQueue<ElemType>::dequeueMax(){
	if(size() == 0)
		Error("Empty heap!");
	swap(0, size() - 1);  
	ElemType maxElem = heap.getAt(size() - 1); // get the largest element 
	heap.removeAt(size() - 1);   // remove the last element
	if(size() != 0)
		heapify(0);    // start heapifying from root node only if more elements exist
	return maxElem;
	
}

/* Returns the size of queue*/
template<typename ElemType>
int PQueue<ElemType>::size(){
	return heap.size();
}

/* Returns the memory used by current queue. Not usually part of class, only for some performance
 * evaluation purposes*/
template<typename ElemType>
int PQueue<ElemType>::bytesUsed(){
	return sizeof(*this) + heap.bytesUsed();
	
}
/* Returns the implementation name */
template<typename ElemType>
string PQueue<ElemType>::implementationName(){
	return "Heap";
}

/*
 * Implementation notes: printDebuggingInfo
 * ----------------------------------------
 * The version for the singly-linked list prints the node structure
 * in order as a debugging aid to keeping track of the cell contents
 * and the pointers between them. It prints each cell in order
 */
template<typename ElemType>
void PQueue<ElemType>::printDebuggingInfo()
{
	cout << "------------------ START DEBUG INFO ------------------" << endl;
	cout << "Pqueue contains " << size() << " entries" << endl;
	for (int i = 0; i < size(); i++) 
		cout << heap[i] << " ";
	cout << endl;
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}


