/*
 * File: pathfinder.cpp
 * --------------------
 * This file provides the source code for the pathfinder problem in assignment 7 of Stanford's 
 * cs106b course. The program is used for two tasks: 
              1. Finding shortest path between two locations.
			  2. Computing minimal spanning tree(based on distance) for a graph.			  
 * Assumption: Graph file is properly formatted
 */

#include <iostream>
#include <fstream>
#include "genlib.h"
#include "simpio.h"
#include "pqueue.h"  // This is a generic priority queue, requires client supplied comparison callback for client's data type 
#include "vector.h"
#include "map.h"
#include "graphics.h"
#include "extgraph.h"
#include "stack.h"
#include "set.h"

/* Constants */
const double NODE_RADIUS = 0.06;   // in inch
const int FONT_SIZE = 10;
const double TEXT_MARGIN = 0.05;
const string DEFAULT_COLOR = "Black";
const string HIGHLIGHT_COLOR = "Red";
const double HIGHLIGHT_INTERVAL = 1;

/* Data structure */
struct nodeT;
struct arcT{
	int distance;  // distance between start and end node
	nodeT *start, *end;
};
struct nodeT{
	string name;
	double x;
	double y;
	Vector<arcT *> arcs;
};

struct pointT{
		double x;
		double y;
};
	


/* ============================ Level 3 Decomposition ==============================
/* Function 1.3.1: DrawNode
 * -------------------------
 * This functions draws the node on the graph with name.
 */
void DrawNode( nodeT * &node, string color){
	string nodeName = node->name;
	SetPenColor(color);
	// Node
	MovePen(node->x + NODE_RADIUS, node->y );
	StartFilledRegion(1);
	DrawArc(NODE_RADIUS, 0, 360); // angle 0->360 for full circle
	EndFilledRegion();
	// Text
	SetPointSize(FONT_SIZE);
	MovePen(node->x + TEXT_MARGIN, node->y + TEXT_MARGIN);
	DrawTextString(nodeName);
}

/* Function 1.3.2: DrawArc
 * -------------------------
 * This functions draws the arc on the graph
 */
void DrawArc(arcT * &arc, string color){
	SetPenColor(color);
	MovePen(arc->start->x, arc->start->y);
	double dx = (arc->end->x) - (arc->start->x);
	double dy = (arc->end->y) - (arc->start->y);
	DrawLine(dx, dy);
}

/* Function 3.1: GetPoint
 * ----------------------
 * This function gets the x and y location of mouse and stores it into the point variable.
 */
void GetPoint(pointT &point){
	point.x= GetMouseX();
	point.y = GetMouseY();
}

/* Function 3.1: GetNodeName
 * ----------------------
 * This function gets the name of the node, if exists, at the current point. Returns empty string otherwise.
 */
string GetNodeName(pointT &point, Map<nodeT *> &nodes){
	Map<nodeT *>::Iterator itr = nodes.iterator(); 
	while(itr.hasNext()){
		string nodeName = itr.next();
		nodeT *node = nodes.getValue(nodeName);
		if(point.x <= node->x + NODE_RADIUS && point.x >= node->x - NODE_RADIUS && point.y <= node->y + NODE_RADIUS &&
			point.y >= node->y - NODE_RADIUS)
			return nodeName;
	}
	return "";
}


/* ============================ Level 2 Decomposition ==============================
 * Function 1.1: HandleData
 * ---------------------------------
 * This function reads the data from the input file stream and saves it into the data structure. Also, it displays all the
 * arcs and nodes side by side.
 */

void HandleData(ifstream &infile, Vector<arcT *> &arcs, Map<nodeT *> &nodes){
	string text;
	infile >> text; // Ideally this should be NODES
	while(true){
		string nodeName;
		infile >> nodeName;  
		if(nodeName == "ARCS") break; 
		nodeT *node = new nodeT;
		node->name = nodeName;
		infile >> node->x >> node->y;
		nodes.add(nodeName, node);
		DrawNode(node, DEFAULT_COLOR);
	}

	// After this arcs will start
	while(true){
		string start;
		infile >> start;
		if(infile.fail()) break; 

		string end;                                     // --------------------------------|
		arcT *arc = new arcT;                           //                                 |
		infile >> end >> arc->distance;                 //                                 |
		arc->start = nodes.getValue(start);             //                                 |
		arc->end = nodes.getValue(end);                 //                                 |
		arc->start->arcs.add(arc);  // add this arc to the arcs of start node       Better not to decompose this code
		arc->end->arcs.add(arc);	// add this arc to the arcs of end node
		arcs.add(arc);                                  //                                 |
		DrawArc(arc, DEFAULT_COLOR);                                   // --------------------------------|
	}
}

/* Function 1.2: DisplayImage
 * ---------------------------------
 * This function displays the map image on graphics screen;
 */
void DisplayImage(string &imageName){
	InitGraphics();
	DrawNamedPicture(imageName);

}

/* Function 3.1: SelectNodes
 * --------------------------
 * This function asks the user for to select start and end location using mouse and saves them into 
 * the vector of nodes passed as argument. Also highlights the nodes.
 */
void SelectNodes(Vector<nodeT *> &endPoints, Map<nodeT *> &nodes){
	for(int i = 0; i < 2; i++){
		while(true){
			if(i == 0) cout << "Click on starting location... " ;
			else cout << "Click on ending location... ";		
			pointT point;
			WaitForMouseDown();
			WaitForMouseUp();
			GetPoint(point);
			string nodeName = GetNodeName(point, nodes);
			if(nodeName != ""){
				if(endPoints.size() == 1 && nodes.getValue(nodeName) == endPoints[0]){
					cout << "Ending location cannot be same as starting location" << endl;
					continue;
				}
				cout << nodeName << " chosen" << endl << endl;
				nodeT * node = nodes.getValue(nodeName);
				DrawNode(node, HIGHLIGHT_COLOR);
				endPoints.add(nodes.getValue(nodeName));
				break;
			}
			cout << "Invalid click! Please try again" << endl;
		}
	}	
}


/* ============================ Level 1 Decomposition ==============================
 * Function 1: UploadNewGraph
 * --------------------------
 * This function does all the necessary steps for uploading a new graph . Post this function the program be in a state
 * ready to asking user for their choice. Concretley, this function accomplish the following tasks:
 *               1. Ask user for a valid graph file.
 *               2. Read the data file and upload the data into the data structure.
 *               3. Display the graph on screen and enable all clicking functionality(to select a location by clicking)
 */
void UploadNewGraph(Vector<arcT *> &arcs, Map<nodeT *> &nodes){
	ifstream in;
	while(true){
		cout << "Please enter name of graph data file: ";
		string filename = GetLine();
		in.open(filename.c_str());
		if (!in.fail()){
			string imageName;
			getline(in,imageName);
			DisplayImage(imageName);
			HandleData(in, arcs, nodes); 
			break;
		}
		else{
			cout << "Invalid filename. Please try again!" << endl << endl;
			in.clear();
		}
	}
	in.close();
}

/* Function 2: GiveOptions
 * --------------------------
 * This function gives the user gameplay options
 */
void GiveOptions(){
	cout << "Your options are:" << endl;
	cout << "           (1) Choose a new graph data file" << endl;
    cout << "           (2) Find shortest path using Dijkstra's algorithm" << endl;
	cout << "           (3) Compute the minimal spanning tree using Kruskal's algorithm" << endl;
	cout << "           (4) Quit" << endl;
	cout << "Enter choice: ";  
}

/* Function 3.1: CmpBySuumedDistance
 * ------------------------
 * This function is the callback function for comparing different elements in the queue.
 * Here, since an element is represented by a stack of arcs, two stacks are compared based on the sum of distance of their arcs
 * The stack with less summed distance is given higher priority
 */
int CmpBySummedDistance(Stack<arcT *> path1, Stack<arcT *> path2){
	int sum1 = 0;
	for(int i = 0; i < path1.size(); i++){
		sum1 += path1.peek()->distance;
		path1.pop();
	}
	int sum2 = 0;
	for(int i = 0; i < path2.size(); i++){
		sum2 += path2.peek()->distance;
		path2.pop();
	}
	
	if(sum1 < sum2)  // Because less sum higher priority
		return 1;
	else if(sum1 == sum2)
		return 0;
	else
		return -1;
}

/* Function 3: GetNewPath
 * --------------------------
 * This function returns a new path to be added to the priority queue if the new possible arc 
 * has either of the end points not already visited and adds the two end points of new arc to the set 
 * of nodes visited. Returns empty path otherwise.
 */
Stack<arcT *> GetNewPath(arcT * &possibleNewArc, Set<string> &nodesVisited, Stack<arcT *> &pathSoFar){
	Stack<arcT *> newPath;
	if(!(nodesVisited.contains(possibleNewArc->start->name)) ||  // if either of the endpoints of possible new 
	!(nodesVisited.contains(possibleNewArc->end->name))){    // arc is not contained in nodesVisited
		newPath = pathSoFar;
		newPath.push(possibleNewArc);
		nodesVisited.add(possibleNewArc->start->name);
		nodesVisited.add(possibleNewArc->end->name);
		return newPath;
	}
	return newPath;
}
/* Function: HighlightPath
 * -----------------------
 * This function highlights the path with the color passed as argument
 */
void HighlightPath(Stack<arcT *> path, string color){
	int pathSize = path.size();
	for(int i = 0; i < pathSize; i++){
		arcT * topArc = path.pop();
		DrawNode(topArc->start, color);
		DrawNode(topArc->end, color);
		DrawArc(topArc, color);
	}
}
/* Function : GetShortestPath
 * --------------------------
 * This function returns the shortest route between the endpoints using Dijkstra's algorithm and returns the stack of nodenames 
 * that are involved in the path. Also prints the number of dequeues done to reach shortest path.
 * Assumes ending location is not the same as starting location.
 */
Stack<arcT *> GetShortestPath(Vector<nodeT *> &endPoints, Map<nodeT *> &nodes){
	Stack<string> shortestPath; 
	PQueue<Stack<arcT *> > pq(CmpBySummedDistance);
	Set<string> nodesVisited;
	// Handle the first search
	for(int i = 0; i < endPoints[0]->arcs.size(); i++){
		Stack<arcT* > emptyPath;
		Stack<arcT *> newPath = GetNewPath(endPoints[0]->arcs[i], nodesVisited, emptyPath);
		if(newPath.size() != 0)
			pq.enqueue(newPath);
	}
	int ndequeue = 0;
	while(true){
		Stack<arcT *> pathToCheck = pq.dequeueMax();
		ndequeue++;
		if(pathToCheck.peek()->end == endPoints[1] || pathToCheck.peek()->start == endPoints[1]){   // because arc is bidirectional
			cout << "The algorithm dequeued " << ndequeue << " paths to find the optimal one." << endl;
			return pathToCheck;
		}
		for(int i = 0; i < pathToCheck.peek()->start->arcs.size(); i++){
			Stack<arcT *> newPath = GetNewPath(pathToCheck.peek()->start->arcs[i], nodesVisited, pathToCheck);
			if(newPath.size() != 0)
				pq.enqueue(newPath);
		}
		for(int i = 0; i < pathToCheck.peek()->end->arcs.size(); i++){
			Stack<arcT *> newPath = GetNewPath(pathToCheck.peek()->end->arcs[i], nodesVisited, pathToCheck);
			if(newPath.size() != 0)
				pq.enqueue(newPath);			
		}
	}
}
/* Function 3: ShortestPath()
 * --------------------------
 * This function is a wrapper function around finding the shortest path
 * It first asks the user for selecting two endPoints, highlights it, uses djikstra's algorithm to get 
 * the shortest path joining those points and highlights the path.
 *
 */
void ShortestPath(Map<nodeT *> &nodes){
	Vector<nodeT *> endPoints;
	SelectNodes(endPoints, nodes);  // Asks user for two endpoints
	cout<< "Finding shortest path using Djikstra's..." << endl;
	Stack<arcT *> path= GetShortestPath(endPoints, nodes);
	HighlightPath(path, HIGHLIGHT_COLOR);
	cout << "Hit return to continue..." ;
	GetLine();
	HighlightPath(path, DEFAULT_COLOR);
	DrawNode(endPoints[0], DEFAULT_COLOR);
	DrawNode(endPoints[1], DEFAULT_COLOR);
}

/* Function : CmpByDistance
 * ------------------------
 * This is the call back function for the peiority queue generated in minimal spanning tree.
 * The arc with shorter distance is given higher priority
 */
int CmpByDistance(arcT *arc1, arcT *arc2){
	if(arc1->distance < arc2->distance)
		return 1;
	else if (arc1->distance == arc2->distance)
		return 0;
	else
		return -1;
}

/* Function: ComputeMinimalSpanningTree
 * ------------------------------------
 * This function is a wrapper around computing the minimal spanning tree. 
 * It finds the path joining the minimal spanning tree using Kruskal's algorithm and then highlights it.
 */
void ComputeMinimalSpanningTree(Vector<arcT *> &arcs){
	PQueue<arcT *> pq(CmpByDistance);
	for(int i = 0; i < arcs.size(); i++)
		pq.enqueue(arcs[i]);
	Set<string> nodesVisited; 
	Vector<arcT *> MinimalSpanningTree;  // Just for dehighighliting purpose
	int distance = 0;
	while(!pq.isEmpty()){
		arcT *nextShortestArc = pq.dequeueMax();
		cout << nextShortestArc->start->name <<"->" <<nextShortestArc->end->name << endl;
		if(!(nodesVisited.contains(nextShortestArc->start->name)) || !(nodesVisited.contains(nextShortestArc->end->name))){
			DrawArc(nextShortestArc, HIGHLIGHT_COLOR);
			//DrawNode(nextShortestArc->start, HIGHLIGHT_COLOR);
			//DrawNode(nextShortestArc->end, HIGHLIGHT_COLOR);
			distance += nextShortestArc->distance;
			nodesVisited.add(nextShortestArc->start->name);
			nodesVisited.add(nextShortestArc->end->name);
		}
	}
	cout << "The minimal spanning tree is highlighted, the total distance is " << distance << endl;
}


		


int main(){
	// Datastructure
	Vector<arcT *> arcs;
	Map<nodeT *> nodes;

	UploadNewGraph(arcs, nodes);  
	while(true){
		GiveOptions();
		string choice = GetLine();
		
		if(choice == "1"){
			cout << endl;
			UploadNewGraph(arcs, nodes);
		}
		else if (choice == "2"){
			ShortestPath(nodes);
		}
		else if (choice == "3")
			ComputeMinimalSpanningTree(arcs);
		else if (choice == "4"){
			cout << "Thanks for playing. Have a nice day!" << endl;
			break;
		}
		else{
			cout << "Invalid choice. Please enter a number between 1 and 4" << endl << endl;
		}

	}
	return 0;

}

// TODO : 
// 1. Replace string with nodes