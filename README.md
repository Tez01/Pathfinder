# Pathfinder
This program finds the shortest path between two nodes selected on a graph. This is a conglomeration of various datastructures and algorithms ranging from vector to maps and priority queues. The two main algorithms used in this program are Dijkstra's algorithm to find shortest path between two points and Kruskal's algorithm to draw the minimal spanning tree for the map.
## Milestone 1:  Make the priority queue generic by using templates in C++
* A heap based priority queue was made generic using templates such that when a new node was added into the distance based priority queue, it is moved up to the appropriate position in the heap using `bubbleUp` routine.
## Milestone 2: Choosing the right datastructures
* Heap was used as the underlying structure to develop a priority queue. 
* The routine `bubbleUp` is central to the correct functioning of the programs as this function rearrange the heap in its proper heap form. Concretely, it rearrange the root node to its  apt position by comparing each parent with its child node and swapping till the parent is either the largestor at the last node. A vector of integers was used to store the childnodes inside this routine.
  Graph data structure was used to store the connectivity as well as additional information for the coneections such as name and distance of the arc.
