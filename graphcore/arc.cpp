#include "arc.h"
#include "graph.h"



Arc::Arc(Graph* parent, Vertex* tail, Vertex* head, QString arcLabel): QObject(parent), arcLabel(arcLabel)
{
	vtail = tail;
	vhead = head;
}

Arc::~Arc()
{

}


Vertex* Arc::tail() const
{
	return vtail;
}

Vertex* Arc::head() const
{
	return vhead;
}

int Arc::index() const
{
	return qobject_cast<Graph*>(parent())->getArcSet().indexOf(const_cast<Arc*>(this));
}

QString Arc::label() const
{
	return arcLabel;
}

void Arc::setLabel(QString newLabel)
{
	arcLabel = newLabel;

	emit labelChanged(newLabel);
}
