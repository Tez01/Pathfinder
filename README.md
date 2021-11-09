# Pathfinder
This program finds the shortest path between two nodes selected on a graph. This is a conglomeration of various datastructures and algorithms ranging from vector to maps and priority queues. The two main algorithms used in this program are Dijkstra's algorithm to find shortest path between two points and Kruskal's algorithm to draw the minimal spanning tree for the map.
## Milestone 1:  Make the priority queue generic by using templates in C++
* A heap based priority queue was made generic using templates such that when a new node was added into the distance based priority queue, it is moved up to the appropriate position in the heap using `bubbleUp` routine.
