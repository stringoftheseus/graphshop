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

QList<Arc*> Vertex::inArcs()
{
	return I;
}

QList<Arc*> Vertex::outArcs()
{
	return O;
}

QList<Arc*> Vertex::arcs()
{
	return O+I;
}


