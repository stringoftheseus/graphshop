#include "graphcore/_graphcore_h.h"
#include "graphrepresentation.h"

GraphRepresentation::GraphRepresentation(Graph* source): QObject(source->parent()), _source(0), _built(false)
{
	_isvalid = false;

	setSource(source);
}

Graph* GraphRepresentation::source()
{
	return _source;
}

void GraphRepresentation::setSource(Graph* source, bool autobuild)
{
	if(_source)
	{
		disconnect(_source, 0, this, 0);
	}

	_graphCleared();

	_source = source;
	_built = false;

	if(autobuild)
	{
		build();
	}
}


void GraphRepresentation::build()
{
	if(_source && !_built)
	{
		const_cast<Graph*>(_source)->build();
		_build();

		_built = true;

		connectGraphSignals();
	}
}


void GraphRepresentation::defaultBuild()
{
	foreach(Vertex* vertex, _source->getVertexSet())
	{
		_vertexAdded(vertex);
	}

	foreach(Edge* edge, _source->getEdgeSet())
	{
		_edgeAdded(edge);
	}

	foreach(Arc* arc, _source->getArcSet())
	{
		_arcAdded(arc);
	}
}

bool GraphRepresentation::valid()
{
	return _isvalid;
}

void GraphRepresentation::validate()
{
	setValid(_valid());
}

void GraphRepresentation::setValid(bool validity)
{
	bool oldvalid = _isvalid;
	_isvalid = validity;

	if(_isvalid != oldvalid)
	{
		emit validityChanged(validity);
	}
}

void GraphRepresentation::connectGraphSignals()
{
	connect(_source, SIGNAL(arcAdded(Arc*)),               SLOT(_arcAdded(Arc*)));
	connect(_source, SIGNAL(arcFlipped(Arc*)),             SLOT(_arcFlipped(Arc*)));
	connect(_source, SIGNAL(arcDeleting(Arc*)),            SLOT(_arcDeleting(Arc*)));
	connect(_source, SIGNAL(arcDeleted(int)),              SLOT(_arcDeleted(int)));
	connect(_source, SIGNAL(arcDeleted(Vertex*, Vertex*)), SLOT(_arcDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(edgeAdded(Edge*)),              SLOT(_edgeAdded(Edge*)));
	connect(_source, SIGNAL(edgeDeleting(Edge*)),           SLOT(_edgeDeleting(Edge*)));
	connect(_source, SIGNAL(edgeDeleted(int)),              SLOT(_edgeDeleted(int)));
	connect(_source, SIGNAL(edgeDeleted(Vertex*, Vertex*)), SLOT(_edgeDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(vertexAdded(Vertex*)),    SLOT(_vertexAdded(Vertex*)));
	connect(_source, SIGNAL(vertexDeleting(Vertex*)), SLOT(_vertexDeleting(Vertex*)));
	connect(_source, SIGNAL(vertexDeleted(int)),      SLOT(_vertexDeleted(int)));

	connect(_source, SIGNAL(graphCleared()), SLOT(_graphCleared()));
}


void GraphRepresentation::_arcAdded(Arc *arc){}
void GraphRepresentation::_arcFlipped(Arc *arc){}
void GraphRepresentation::_arcDeleting(Arc *arc){}
void GraphRepresentation::_arcDeleted(int arc){}
void GraphRepresentation::_arcDeleted(Vertex*, Vertex*){}

void GraphRepresentation::_edgeAdded(Edge *edge){}
void GraphRepresentation::_edgeDeleting(Edge *edge){}
void GraphRepresentation::_edgeDeleted(int edge){}
void GraphRepresentation::_edgeDeleted(Vertex*, Vertex*){}

void GraphRepresentation::_vertexAdded(Vertex *vertex){}
void GraphRepresentation::_vertexDeleting(Vertex *vertex){}
void GraphRepresentation::_vertexDeleted(int vertex){}

void GraphRepresentation::_graphCleared(){}
