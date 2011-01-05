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

QList<Interval*> IntervalGraph::intersetions(Interval* interval)
{
	return _intersections.values(interval);
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

	foreach(Interval* other, _intersections.values(interval))
	{
		if(interval->intersects(other) == false)
		{
			_intersections.remove(interval, other);
			emit intersectionLost(interval, other);
		}
	}

	foreach(Interval* other, _intervals)
	{
		if(_intersections.contains(interval, other) == false && interval->intersects(other))
		{
			_intersections.insert(interval, other);
			emit intersectionMade(interval, other);
		}
	}
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

	foreach(Interval* intersect, intersections(interval))
	{
		emit intersectionLost(interval, intersect);
	}

	_intervals.remove(vertex);
}

void IntervalGraph::_graphCleared()
{
	_intervals.clear();

	emit cleared();
}
