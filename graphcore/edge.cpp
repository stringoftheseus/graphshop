#include "_graphcore_h.h"

Edge::Edge(Graph* parent, Vertex* vertex1, Vertex* vertex2, QString edgeLabel): QObject(parent), edgeLabel(edgeLabel)
{
	v1 = vertex1;
	v2 = vertex2;
}

Edge::~Edge()
{

}


Vertex* Edge::vertex1() const
{
	return v1;
}

Vertex* Edge::vertex2() const
{
	return v2;
}

int Edge::index() const
{
	return qobject_cast<Graph*>(parent())->getEdgeSet().indexOf(const_cast<Edge*>(this));
}

QString Edge::label() const
{
	return edgeLabel;
}

void Edge::setLabel(QString newLabel)
{
	edgeLabel = newLabel;

	emit labelChanged(newLabel);
}
