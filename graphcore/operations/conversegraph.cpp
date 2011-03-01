#include "graphcore/_graphcore_h.h"

#include "conversegraph.h"

ConverseGraph::ConverseGraph(const Graph *source): GraphOperation(source)
{
	updateLabel();
}

QScriptValue ConverseGraph::sConverseGraph(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new ConverseGraph(qobject_cast<Graph*>(context->argument(0).toQObject())));
}

Graph* ConverseGraph::of(const Graph* source)
{
	ConverseGraph conv(source);
	conv.build();

	return conv.clone();
}


void ConverseGraph::_build()
{
	defaultBuild();
}

void ConverseGraph::arcAdded(Arc *arc)
{
	addArc(arc->head()->index(), arc->tail()->index(), arc->label());
}

void ConverseGraph::arcFlipped(Arc *arc)
{
	flipArc(getArc(arc->index()));
}

void ConverseGraph::arcDeleted(int index)
{
	removeArc(getArc(index));
}

void ConverseGraph::edgeAdded(Edge *edge)
{
	addEdge(edge->vertex1(), edge->vertex2(), edge->label());
}

void ConverseGraph::edgeDeleted(int index)
{
	removeEdge(getEdge(index));
}

void ConverseGraph::vertexAdded(Vertex *vertex)
{
	addVertex(vertex->label());
}

void ConverseGraph::vertexDeleted(int index)
{
	removeVertex(getVertex(index));
}

void ConverseGraph::graphCleared()
{
	clear();
}

void ConverseGraph::updateLabel()
{
	setLabel("conv(" +_source->label()+")");
}
