#include "graphcore/vertex.h"

#include "interval.h"

Interval::Interval(double left, double right, Vertex* source): QObject(source->parent())
{
	_ledge = left;
	_redge = right;

	_source = source;
}

Vertex* Interval::sourceVertex()
{
	return _source;
}

int Interval::index()
{
	return _source->index();
}

double Interval::width()
{
	return rightEdge() - leftEdge();
}

double Interval::center()
{
	return (rightEdge() + leftEdge()) / 2.0;
}

double Interval::leftEdge()
{
	return _ledge;
}

double Interval::rightEdge()
{
	return _redge;
}

bool Interval::intersects(Interval* that)
{
	return (qMax(leftEdge(), that->leftEdge()) <= qMin(rightEdge(), that->rightEdge()));
}

void Interval::centerOn(double point)
{
	move(point - center());
}

void Interval::moveLeftEdge(double delta)
{
	setEdges(leftEdge()+delta, rightEdge());
}

void Interval::moveRightEdge(double delta)
{
	setEdges(leftEdge(), rightEdge()+delta);
}

void Interval::move(double delta)
{
	setEdges(leftEdge()+delta, rightEdge()+delta);
}

void Interval::setLeftEdge(double left)
{
	setEdges(left, rightEdge());
}

void Interval::setRightEdge(double right)
{
	setEdges(leftEdge(), right);
}

void Interval::setEdges(double left, double right)
{
	_ledge = left;
	_redge = right;

	normalize();

	emit moved(this);
}

void Interval::normalize()
{
	if(_ledge > _redge)
	{
		qSwap(_ledge, _redge);
	}
}
