#include "graphcore/_graphcore_h.h"
#include "complementgraph.h"

ComplementGraph::ComplementGraph(const Graph *source): GraphOperation(source)
{
	updateLabel();
}


QScriptValue ComplementGraph::sComplementGraph(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new ComplementGraph(qobject_cast<Graph*>(context->argument(0).toQObject())));
}

Graph* ComplementGraph::of(const Graph* source)
{
	ComplementGraph comp(source);

	return comp.clone();
}


void ComplementGraph::_build()
{
	synchronizeVertices();

	for(int v1=0; v1<vertexCount(); v1++)
	{
		for(int v2=v1+1; v2<vertexCount(); v2++)
		{
			updateEdgeCandidate(v1, v2);
		}
	}
}

void ComplementGraph::updateEdgeCandidate(int vertex1, int vertex2)
{
	setEdgeMultiplicity(vertex1, vertex2, _source->hasEdge(vertex1, vertex2) ? 0 : 1);
}

void ComplementGraph::updateEdgeCandidate(Vertex* vertex1, Vertex* vertex2)
{
	setEdgeMultiplicity(vertex1, vertex2, _source->hasEdge(vertex1, vertex2) ? 0 : 1);
}


void ComplementGraph::edgeAdded(Edge* edge)
{
	updateEdgeCandidate(edge->vertex1()->index(), edge->vertex2()->index());
}

void ComplementGraph::edgeDeleted(Vertex* v1, Vertex* v2)
{
	updateEdgeCandidate(v1->index(), v2->index());
}

void ComplementGraph::vertexAdded(Vertex* vertex)
{
	addVertex(vertex->label());


}

void ComplementGraph::vertexDeleted(int index)
{
	removeVertex(getVertex(index));
}

void ComplementGraph::graphCleared()
{
	clear();
}

void ComplementGraph::updateLabel()
{
	setLabel("compl(" +_source->label()+")");
}
