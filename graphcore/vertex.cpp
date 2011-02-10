#include "vertex.h"

#include "edge.h"
#include "arc.h"
#include "graph.h"

Vertex::Vertex(Graph *parent, QString vertexLabel): QObject(parent), graph(parent), vertexLabel(vertexLabel)
{
}

Vertex::~Vertex()
{

}


QList<Vertex*> Vertex::neighborhood()
{
	QList<Vertex*> Ng;

	for(int i=0; i<E.size(); i++)
	{
		Edge* e = E[i];
		Vertex* v = (e->vertex1() == this ? e->vertex2() : e->vertex1());

		if(Ng.contains(v) == false)
		{
			Ng.append(v);
		}

	}

	for(int i=0; i<I.size(); i++)
	{
		if(Ng.contains(I[i]->tail()) == false)
		{
			Ng.append(I[i]->tail());
		}
	}

	for(int i=0; i<O.size(); i++)
	{
		if(Ng.contains(O[i]->head()) == false)
		{
			Ng.append(O[i]->head());
		}
	}

	return Ng;
}

QList<Vertex*> Vertex::inNeighborhood()
{
	QList<Vertex*> Ng;

	for(int i=0; i<I.size(); i++)
	{
		if(Ng.contains(I[i]->tail()) == false)
		{
			Ng.append(I[i]->tail());
		}
	}

	return Ng;
}

QList<Vertex*> Vertex::outNeighborhood()
{
	QList<Vertex*> Ng;

	for(int i=0; i<O.size(); i++)
	{
		if(Ng.contains(O[i]->head()) == false)
		{
			Ng.append(O[i]->head());
		}
	}

	return Ng;
}


QString Vertex::label() const
{
	return vertexLabel;
}

void Vertex::setLabel(QString newLabel)
{
	vertexLabel = newLabel;

	emit labelChanged(newLabel);
}

int Vertex::index() const
{
	return graph->getVertexSet().indexOf(const_cast<Vertex*>(this));
}

QList<Edge*> Vertex::edges()
{
	return E;
}

QList<Edge*> Vertex::edgesWith(Vertex *other)
{
	QList<Edge*> W;

	foreach(Edge* edge, E)
	{
		if(edge->vertex1() == other || edge->vertex2() == other)
		{
			W.append(edge);
		}
	}

	return W;
}



QList<Arc*> Vertex::inArcs()
{
	return I;
}

QList<Arc*> Vertex::arcsFrom(Vertex *other)
{
	QList<Arc*> W;

	foreach(Arc* arc, I)
	{
		if(arc->tail() == other)
		{
			W.append(arc);
		}
	}

	return W;
}


QList<Arc*> Vertex::outArcs()
{
	return O;
}

QList<Arc*> Vertex::arcsTo(Vertex *other)
{
	QList<Arc*> W;

	foreach(Arc* arc, O)
	{
		if(arc->head() == other)
		{
			W.append(arc);
		}
	}

	return W;
}

QList<Arc*> Vertex::arcs()
{
	return O+I;
}


QList<Arc*> Vertex::arcsWith(Vertex *other)
{
	QList<Arc*> W;

	foreach(Arc* arc, O+I)
	{
		if(arc->head() == other || arc->tail() == other)
		{
			W.append(arc);
		}
	}

	return W;
}
