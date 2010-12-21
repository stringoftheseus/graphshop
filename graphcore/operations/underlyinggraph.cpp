#include "graphcore/_graphcore_h.h"
#include "underlyinggraph.h"

UnderlyingGraph::UnderlyingGraph(const Graph *source): GraphOperation(source)
{
	updateLabel();
}


QScriptValue UnderlyingGraph::sUnderlyingGraph(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new UnderlyingGraph(qobject_cast<Graph*>(context->argument(0).toQObject())));
}

Graph* UnderlyingGraph::of(const Graph* source)
{
	UnderlyingGraph under(source);

	return under.clone();
}


void UnderlyingGraph::_build()
{
	defaultBuild();
}

void UnderlyingGraph::arcAdded(Arc* arc)
{
	_arcs.insert(arc, addEdge(arc->tail()->index(), arc->head()->index(), arc->label()));
}

void UnderlyingGraph::arcDeleting(Arc* arc)
{
	removeEdge(_arcs.value(arc));
}

void UnderlyingGraph::edgeAdded(Edge* edge)
{
	_edges.insert(edge, addEdge(edge->vertex1()->index(), edge->vertex2()->index(), edge->label()));
}

void UnderlyingGraph::edgeDeleting(Edge* edge)
{
	removeEdge(_edges.value(edge));
}

void UnderlyingGraph::vertexAdded(Vertex* vertex)
{
	addVertex(vertex->label());
}

void UnderlyingGraph::vertexDeleted(int index)
{
	removeVertex(getVertex(index));
}

void UnderlyingGraph::graphCleared()
{
	_arcs.clear();
	_edges.clear();

	clear();
}

void UnderlyingGraph::updateLabel()
{
	setLabel("under(" +_source->label()+")");
}
