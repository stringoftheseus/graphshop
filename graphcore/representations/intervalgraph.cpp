#include "graphcore/_graphcore_h.h"
#include "interval.h"

#include "intervalgraph.h"

IntervalGraph::IntervalGraph(Graph* source): GraphRepresentation(source)
{

}

IntervalGraph::IntervalGraph(QObject *parent): GraphRepresentation(new Graph(parent))
{

}

int IntervalGraph::intervalCount()
{
	return _intervals.count();
}

Interval* IntervalGraph::getInterval(int index)
{
	return _intervals.values().at(index);
}

void IntervalGraph::addInterval(double left, double right)
{
	_working = true;
	Vertex* vertex = _source->addVertex();
	_working = false;

	addInterval(left, right, vertex);
}

void IntervalGraph::addInterval(double left, double right, Vertex *vertex)
{
	Interval* interval = new Interval(left, right, vertex);

	_intervals.insert(vertex, interval);

	if(right > _endpoint)
	{
		_endpoint = right;
	}

	connect(interval, SIGNAL(moved(Interval*)), SLOT(checkEdges(Interval*)));

	emit intervalAdded(interval);
}

void IntervalGraph::deleteInterval(Interval* interval)
{
	_source->removeVertex(interval->sourceVertex());
}

bool IntervalGraph::_valid()
{
	// TODO: Implement this
	return false;
}

void IntervalGraph::_build()
{
	_working = false;
	_endpoint = 0;

	// TODO: snychronize vertices...

	validate(); // _valid sets interval positions if it is valid
}



void IntervalGraph::_intervalMoved(Interval* interval)
{
	foreach(Interval* other, _intervals)
	{
		_source->setEdgeMultiplicity(interval->sourceVertex(), other->sourceVertex(), interval->intersects(other));
	}

	emit intervalMoved(interval);
}

void IntervalGraph::_edgeAdded(Edge*)
{
	validate();
}

void IntervalGraph::_edgeDeleted(int)
{
	validate();
}

void IntervalGraph::_vertexAdded(Vertex* vertex)
{
	if(!_working)
	{
		addInterval(_endpoint+1, _endpoint+2, vertex);
	}
}

void IntervalGraph::_vertexDeleting(Vertex* vertex)
{
	Interval* interval = _intervals[vertex];

	emit intervalDeleting(interval);

	_intervals.remove(vertex);
}

void IntervalGraph::_graphCleared()
{
	_intervals.clear();

	emit cleared();
}
