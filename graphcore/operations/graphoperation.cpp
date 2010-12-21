#include "graphcore/_graphcore_h.h"
#include "graphoperation.h"

GraphOperation::GraphOperation(Graph const* source): Graph(source->parent()), _source(0), _built(false)
{
	setSource(source);
}

void GraphOperation::setSource(Graph const* source, bool autobuild)
{
	if(_source)
	{
		disconnect(_source, 0, this, 0);
	}

	graphCleared();

	_source = source;
	_built = false;

	if(autobuild)
	{
		build();
	}
}


void GraphOperation::build()
{
	if(_source && !_built)
	{
		const_cast<Graph*>(_source)->build();
		_build();

		_built = true;

		connectGraphSignals();
	}
}

void GraphOperation::defaultBuild()
{
	foreach(Vertex* vertex, _source->getVertexSet())
	{
		vertexAdded(vertex);
	}

	foreach(Edge* edge, _source->getEdgeSet())
	{
		edgeAdded(edge);
	}

	foreach(Arc* arc, _source->getArcSet())
	{
		arcAdded(arc);
	}

	updateLabel();
}

void GraphOperation::synchronizeVertices()
{
	for(int i=vertexCount(); i<_source->vertexCount(); i++)
	{
		addVertex(_source->getVertex(i)->label());
	}

	while(vertexCount() > _source->vertexCount())
	{
		removeVertex(getVertex(vertexCount()-1));
	}
}

void GraphOperation::connectGraphSignals()
{
	connect(_source, SIGNAL(arcAdded(Arc*)),               SLOT(arcAdded(Arc*)));
	connect(_source, SIGNAL(arcFlipped(Arc*)),             SLOT(arcFlipped(Arc*)));
	connect(_source, SIGNAL(arcDeleting(Arc*)),            SLOT(arcDeleting(Arc*)));
	connect(_source, SIGNAL(arcDeleted(int)),              SLOT(arcDeleted(int)));
	connect(_source, SIGNAL(arcDeleted(Vertex*, Vertex*)), SLOT(arcDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(edgeAdded(Edge*)),              SLOT(edgeAdded(Edge*)));
	connect(_source, SIGNAL(edgeDeleting(Edge*)),           SLOT(edgeDeleting(Edge*)));
	connect(_source, SIGNAL(edgeDeleted(int)),              SLOT(edgeDeleted(int)));
	connect(_source, SIGNAL(edgeDeleted(Vertex*, Vertex*)), SLOT(edgeDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(vertexAdded(Vertex*)),    SLOT(vertexAdded(Vertex*)));
	connect(_source, SIGNAL(vertexDeleting(Vertex*)), SLOT(vertexDeleting(Vertex*)));
	connect(_source, SIGNAL(vertexDeleted(int)),      SLOT(vertexDeleted(int)));

	connect(_source, SIGNAL(graphCleared()), SLOT(graphCleared()));

	connect(_source, SIGNAL(labelChanged(QString)), SLOT(updateLabel()));
}


void GraphOperation::arcAdded(Arc *arc){}
void GraphOperation::arcFlipped(Arc *arc){}
void GraphOperation::arcDeleting(Arc *arc){}
void GraphOperation::arcDeleted(int arc){}
void GraphOperation::arcDeleted(Vertex*, Vertex*){}

void GraphOperation::edgeAdded(Edge *edge){}
void GraphOperation::edgeDeleting(Edge *edge){}
void GraphOperation::edgeDeleted(int edge){}
void GraphOperation::edgeDeleted(Vertex*, Vertex*){}

void GraphOperation::vertexAdded(Vertex *vertex){}
void GraphOperation::vertexDeleting(Vertex *vertex){}
void GraphOperation::vertexDeleted(int vertex){}

void GraphOperation::graphCleared(){}

void GraphOperation::updateLabel(){}
