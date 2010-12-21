#include "graphcore/_graphcore_h.h"

#include "dominationgraph.h"

DominationGraph::DominationGraph(const Graph *source): GraphOperation(source)
{
	updateLabel();
}

QScriptValue DominationGraph::sDominationGraph(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new DominationGraph(qobject_cast<Graph*>(context->argument(0).toQObject())));
}

Graph* DominationGraph::of(Graph const* source)
{
	DominationGraph dom(source);

	return dom.clone();
}

void DominationGraph::_build()
{
	synchronizeVertices();

	for(int u=0; u<_source->vertexCount(); u++)
	{
		for(int v = u+1; v<_source->vertexCount(); v++)
		{
			if(isDominantPair(_source, u, v))
			{
				addEdge(u, v);
			}
		}
	}

	updateLabel();
}

void DominationGraph::rebuild()
{
	removeEdges();
	_build();
}

bool DominationGraph::isDominantPair(Graph const* graph, int vertex1, int vertex2)
{
	return isDominantPair(graph, graph->getVertex(vertex1), graph->getVertex(vertex2));
}

bool DominationGraph::isDominantPair(Graph const* graph, Vertex* vertex1, Vertex* vertex2)
{
	int vc = graph->vertexCount();

	QList<Vertex*> vs1 = vertex1->outNeighborhood();
	QList<Vertex*> vs2 = vertex2->outNeighborhood();

	QList<Vertex*> vs;
	vs.append(vs1);
	vs.append(vs2);

	if(vs.count() < vc-2)
	{
		return false;
	}
	else
	{
		int dc = 0;
		for(int v=0; v<vc; v++)
		{
			Vertex* vertex = graph->getVertex(v);

			if(vertex == vertex1 || vertex == vertex2 || vs.contains(vertex))
			{
				dc++;
			}
		}

		return (dc == vc);
	}
}


void DominationGraph::arcAdded(Arc *arc)
{
	rebuild();
}

void DominationGraph::arcFlipped(Arc *arc)
{
	rebuild();
}

void DominationGraph::arcDeleted(int index)
{
	rebuild();
}

void DominationGraph::edgeAdded(Edge *edge)
{
	/* TODO: The general definition of a domination graph includes one for plain graphs
			 (edges instead of arcs). It would probably be a good idea to look that up
			 and see how it might effect this...
	*/
}

void DominationGraph::edgeDeleted(int index)
{
}

void DominationGraph::vertexAdded(Vertex *vertex)
{
	rebuild();
}

void DominationGraph::vertexDeleted(int index)
{
	rebuild();
}

void DominationGraph::graphCleared()
{
	clear();
}

void DominationGraph::updateLabel()
{
	setLabel("dom(" +_source->label()+")");
}
