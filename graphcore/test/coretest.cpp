#include <iostream>
#include "../../graphio/graphiotext.h"
#include "../../graphlib/graphclass/classcomplete.h"



using namespace std;
using namespace GraphShop;

int main()
{
	cout << "Hello GraphWorld" << endl;
	
	//Graph* myGraph = GraphIOText::fromFile("C:/Aaron/svn/Aaron/graphshop/trunk/graphio/samples/adjlist01.txt");

	Graph myGraph;
	
	// TODO: How about Graph::addVertex(int count) that adds count of them...
	myGraph.addVertex();
	myGraph.addVertex();
	myGraph.addVertex();
	myGraph.addVertex();
	
	myGraph.addEdge(0, 1);
	myGraph.addEdge(1, 2);
	myGraph.addEdge(2, 0);
	myGraph.addEdge(3, 0);
	myGraph.addEdge(3, 1);
	myGraph.addEdge(2, 3);


	cout << classComplete::graphIs(myGraph) << endl;

	//char* path = "C:/Aaron/svn/Aaron/graphshop/trunk/graphio/samples/adjmatrixOut03.txt";
	//GraphIOText::toFile(myGraph, path, GraphIOText::ADJ_LIST);
}
