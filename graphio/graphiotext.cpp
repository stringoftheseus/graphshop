#include "graphiotext.h"
#include "../graphcore/edge.h"
#include "../graphcore/arc.h"
#include "../graphcore/vertex.h"

#include <sstream>
#include <fstream>

#define ENDL "\r\n"

using std::string;

void GraphIOText::fromString(Graph& graph, const char* data)
{
	fromString(graph, string(data));
}

void GraphIOText::fromString(Graph& graph, string const& data)
{
	std::istringstream inStream(data, std::ios_base::in);
	GraphIOText::fromStream(graph, &inStream);
}

void GraphIOText::fromStream(Graph& graph, std::istream* in)
{
	string type = getLine(in);
	string mode = getLine(in);

	if(mode == "AdjacencyList")
	{
		parseAdjList(graph, in);
	}
	else if(mode == "AdjacencyMatrix")
	{
		if(type == "Graph")
		{
			parseGraphAdjMatrix(graph, in);
		}
		else if(type == "Digraph")
		{
			parseDigraphAdjMatrix(graph, in);
		}
		else
		{
			throw "unknown graph type";
		}
	}
	else
	{
		throw "unknown graph input mode";
	}
}

void GraphIOText::fromFile(Graph& graph, const char* filename)
{
	std::ifstream in(filename, std::ios_base::in);
	fromStream(graph, &in);
}


void GraphIOText::parseAdjList(Graph& graph, std::istream* in)
{
	while(!in->eof())
	{
		char start, end, sep;
		int v1, v2;

		*in >> start >> v1 >> sep >> v2 >> end;

		if(start == '{')
		{
			graph.addEdge(v1, v2);
		}
		else if(start == '(')
		{
			graph.addArc(v1, v2);
		}
		else
		{
			throw "bad edge type in adjacency matrix";
		}
	}
}

void GraphIOText::parseGraphAdjMatrix(Graph& graph, std::istream* in)
{
	int edges;

	for(int r = 0;!in->eof(); r++)
	{
		string line = getLine(in);

		graph.addVertex();
		std::istringstream linein(line, std::ios_base::in);

		for(int c = 0;c <= r; c++)
		{
			linein >> edges;

			for(int e=0; e<edges; e++)
			{
				graph.addEdge(r, c);
			}
		}
	}
}

void GraphIOText::parseDigraphAdjMatrix(Graph& graph, std::istream* in)
{
	int arcs;

	for(int r = 0;!in->eof(); r++)
	{
		string line = getLine(in);

		std::istringstream linein(line, std::ios_base::in);

		for(int c = 0;!linein.eof(); c++)
		{
			if(r == 0)
			{
				graph.addVertex();
			}
			
			linein >> arcs;

			for(int a=0; a<arcs; a++)
			{
				graph.addArc(r, c);
			}
		}
	}
}

string GraphIOText::getLine(std::istream* in)
{
	string data;
	std::getline(*in, data);

	if(data[data.length()-1] == '\r')
	{
		data = data.substr(0, data.length()-1);
	}

	return data;
}





string GraphIOText::toString(Graph const& graph, Mode mode)
{
	string data;

	std::ostringstream outStream(data, std::ios_base::out);
	toStream(graph, &outStream, mode);

	return data;
}

void GraphIOText::toStream(Graph const& graph, std::ostream* out, Mode mode)
{
	string type;

	if(graph.getArcSet().size() == 0)
	{
		*out << "Graph";
	}
	else if(graph.getEdgeSet().size() == 0)
	{
		*out << "Digraph";
	}
	else
	{
		*out << "MixedGraph";
	}

	if(mode == ADJ_LIST)
	{
		*out << ENDL << "AdjacencyList";
		buildAdjList(graph, out);
	}
	else if(mode == ADJ_MATRIX)
	{
		*out << ENDL << "AdjacencyMatrix";
		
		if(graph.getArcSet().size() == 0)
		{
			buildGraphAdjMatrix(graph, out);
		}
		else if(graph.getEdgeSet().size() == 0)
		{
			buildDigraphAdjMatrix(graph, out);
		}
		else
		{
			throw "I haven't figured out how to make an adjacency matrix for a mixed graph yet :(";
		}
	}
	else
	{
		throw "unknown graph export mode";
	}
}

void GraphIOText::toFile(Graph const& graph, const char* filename, Mode mode)
{
	std::ofstream out(filename, std::ios_base::out);
	toStream(graph, &out, mode);
}


void GraphIOText::buildGraphAdjMatrix(Graph const& graph, std::ostream* out)
{
	int vertices = graph.getVertexSet().size();

	for(int r=0; r<vertices; r++)
	{
		*out << ENDL;

		for(int c=0; c<vertices; c++)
		{
			if(c > 0)
			{
				*out << ' ';
			}

			*out << graph.getEdges(r, c).size();
		}
	}
}

void GraphIOText::buildDigraphAdjMatrix(Graph const& graph, std::ostream* out)
{
	int vertices = graph.getVertexSet().size();

	for(int r=0; r<vertices; r++)
	{
		*out << ENDL;

		for(int c=0; c<vertices; c++)
		{
			if(c > 0)
			{
				*out << ' ';
			}

			*out << graph.getArcs(r, c).size();
		}
	}
}

void GraphIOText::buildAdjList(Graph const& graph, std::ostream* out)
{
	for(int e=0; e<graph.edgeCount(); e++)
	{
		Edge const* edge = graph.getEdge(e);
		*out << ENDL << '{' << edge->vertex1()->index() << ',' << edge->vertex2()->index() << '}';
	}

	for(int a=0; a<graph.arcCount(); a++)
	{
		Arc const* arc = graph.getArc(a);
		*out << ENDL << '(' << arc->tail()->index() << ',' << arc->head()->index() << ')';
	}
}
