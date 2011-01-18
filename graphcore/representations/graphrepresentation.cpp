#include "graphcore/_graphcore_h.h"
#include "graphrepresentation.h"

GraphRepresentation::GraphRepresentation(Graph* source): QObject(source->parent()), _source(0), _built(false)
{
	_isvalid = false;
	_ignoreSignals = false;

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
	connect(_source, SIGNAL(arcAdded(Arc*)),               SLOT(_s_arcAdded(Arc*)));
	connect(_source, SIGNAL(arcFlipped(Arc*)),             SLOT(_s_arcFlipped(Arc*)));
	connect(_source, SIGNAL(arcDeleting(Arc*)),            SLOT(_s_arcDeleting(Arc*)));
	connect(_source, SIGNAL(arcDeleted(int)),              SLOT(_s_arcDeleted(int)));
	connect(_source, SIGNAL(arcDeleted(Vertex*, Vertex*)), SLOT(_s_arcDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(edgeAdded(Edge*)),              SLOT(_s_edgeAdded(Edge*)));
	connect(_source, SIGNAL(edgeDeleting(Edge*)),           SLOT(_s_edgeDeleting(Edge*)));
	connect(_source, SIGNAL(edgeDeleted(int)),              SLOT(_s_edgeDeleted(int)));
	connect(_source, SIGNAL(edgeDeleted(Vertex*, Vertex*)), SLOT(_s_edgeDeleted(Vertex*,Vertex*)));

	connect(_source, SIGNAL(vertexAdded(Vertex*)),    SLOT(_s_vertexAdded(Vertex*)));
	connect(_source, SIGNAL(vertexDeleting(Vertex*)), SLOT(_s_vertexDeleting(Vertex*)));
	connect(_source, SIGNAL(vertexDeleted(int)),      SLOT(_s_vertexDeleted(int)));

	connect(_source, SIGNAL(graphCleared()), SLOT(_s_graphCleared()));
}

void GraphRepresentation::ignoreSignals(bool ignore)
{
	_ignoreSignals = ignore;
}

bool GraphRepresentation::signalsIgnored()
{
	return _ignoreSignals;
}

void GraphRepresentation::_s_arcAdded(Arc *arc){if(!_ignoreSignals) _arcAdded(arc);}
void GraphRepresentation::_s_arcFlipped(Arc *arc){if(!_ignoreSignals) _arcFlipped(arc);}
void GraphRepresentation::_s_arcDeleting(Arc *arc){if(!_ignoreSignals) _arcDeleting(arc);}
void GraphRepresentation::_s_arcDeleted(int arc){if(!_ignoreSignals) _arcDeleted(arc);}
void GraphRepresentation::_s_arcDeleted(Vertex* v1, Vertex* v2){if(!_ignoreSignals) _arcDeleted(v1, v2);}

void GraphRepresentation::_s_edgeAdded(Edge *edge){if(!_ignoreSignals) _edgeAdded(edge);}
void GraphRepresentation::_s_edgeDeleting(Edge *edge){if(!_ignoreSignals) _edgeDeleting(edge);}
void GraphRepresentation::_s_edgeDeleted(int edge){if(!_ignoreSignals) _edgeDeleted(edge);}
void GraphRepresentation::_s_edgeDeleted(Vertex* v1, Vertex* v2){if(!_ignoreSignals) _edgeDeleted(v1, v2);}

void GraphRepresentation::_s_vertexAdded(Vertex *vertex){if(!_ignoreSignals) _vertexAdded(vertex);}
void GraphRepresentation::_s_vertexDeleting(Vertex *vertex){if(!_ignoreSignals) _vertexDeleting(vertex);}
void GraphRepresentation::_s_vertexDeleted(int vertex){if(!_ignoreSignals) _vertexDeleted(vertex);}

void GraphRepresentation::_s_graphCleared(){if(!_ignoreSignals) _graphCleared();}


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
