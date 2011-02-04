#include "tournament.h"

#include "graphcore/arc.h"
#include "graphcore/vertex.h"

Tournament::Tournament(Graph* source): GraphRepresentation(source)
{
}

Tournament::Tournament(QObject *parent): GraphRepresentation(new Graph(parent))
{
}

void Tournament::_build()
{
	validate();
}

int Tournament::addVertex()
{
	ignoreSignals(true);

	Vertex* added = _source->addVertex();

	foreach(Vertex* vertex, _source->getVertexSet())
	{
		if(vertex != added)
		{
			_source->addArc(vertex, added);
		}
	}

	ignoreSignals(false);

	validate();
	emit vertexAdded(added->index());

	return added->index();
}

void Tournament::removeVertex(int index)
{
	_source->removeVertex(_source->getVertex(index));
}

int Tournament::order()
{
	return _source->vertexCount();
}

bool Tournament::hasArc(int a, int b)
{
	return _source->hasArc(a, b);
}

void Tournament::setArc(int a, int b)
{
	if(!hasArc(a, b))
	{
		flipArc(a, b);
	}
}

void Tournament::flipArc(int a, int b)
{
	if(hasArc(a, b))
	{
		_source->flipArc(_source->getArcs(a, b)[0]);
	}
	else if(hasArc(b, a))
	{
		_source->flipArc(_source->getArcs(b, a)[0]);
	}
}

QList<int> Tournament::scoreSequence()
{
	QList<int> scores;

	foreach(Vertex* v, _source->getVertexSet())
	{
		scores.append(v->outArcs().count());
	}

	qSort(scores);

	return scores;
}

bool Tournament::_valid()
{
	for(int i = 0; i < _source->vertexCount(); i++)
	{
		if(_source->getArcs(i, i).count() != 0)
		{
			return false;
		}

		for(int j = i+1; j < _source->vertexCount(); j++)
		{
			if(_source->getArcs(i, j).count() + _source->getArcs(j, i).count() != 1)
			{
				return false;
			}
		}
	}

	return true;
}

void Tournament::_arcAdded(Arc*)
{
	validate();
}

void Tournament::_arcFlipped(Arc* arc)
{
	emit arcFlipped(arc->tail()->index(), arc->head()->index());
}

void Tournament::_arcDeleted(int)
{
	validate();
}

void Tournament::_vertexAdded(Vertex* vertex)
{
	validate();
	emit vertexAdded(vertex->index());
}

void Tournament::_vertexDeleting(Vertex* vertex)
{
	//ignoreSignals(true);
}

void Tournament::_vertexDeleted(int index)
{
	//ignoreSignals(false);

	emit vertexDeleted(index);
	validate();	
}

void Tournament::_graphCleared()
{
	emit cleared();
}
