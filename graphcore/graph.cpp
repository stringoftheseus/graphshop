#include "graphcore/_graphcore_h.h"
#include "graphcore/operations/_graphoperations_h.h"


Graph::Graph(QObject *parent): QObject(parent), graphLabel("")
{

}

Graph::Graph(QString graphLabel, QObject *parent): QObject(parent), graphLabel(graphLabel)
{

}

Graph::~Graph()
{

}

Graph* Graph::clone()
{
	build();

	Graph* that = new Graph(label(), parent());

	foreach(Vertex* v, getVertexSet())
	{
		that->addVertex(v->label());
	}

	foreach(Edge* e, getEdgeSet())
	{
		that->addEdge(e->vertex1(), e->vertex2(), e->label());
	}

	foreach(Arc* a, getArcSet())
	{
		that->addArc(a->tail(), a->head(), a->label());
	}

	return that;
}

void Graph::build() {}

QScriptValue Graph::sGraph(QScriptContext *context, QScriptEngine *engine)
{
	Graph* graph;

	if(context->argumentCount() == 0)
	{
		graph = new Graph(engine);
	}
	else
	{
		graph = new Graph(context->argument(0).toString(), engine);
	}

	return engine->newQObject(graph);
}



// TODO: need addVertex(int) that adds int vertices...
Vertex* Graph::addVertex(QString vertexLabel)
{
	Vertex* vertex = new Vertex(this, vertexLabel);
	V.append(vertex);

	emit vertexAdded(vertex);

	return vertex;
}

void Graph::removeVertex(Vertex* vertex)
{
	emit vertexDeleting(vertex);

	foreach(Arc* arc, vertex->arcs())
	{
		removeArc(arc);
	}

	foreach(Edge* edge, vertex->edges())
	{
		removeEdge(edge);
	}


	int index = vertex->index();
	V.removeAll(vertex);

	emit vertexDeleted(index);
}

void Graph::removeVertices()
{
	while(!V.isEmpty())
	{
		removeVertex(V.takeFirst());
	}
}


QList<Vertex*> Graph::getVertexSet() const
{
	return V;
}

Vertex* Graph::getVertex(int index) const
{
	return V[index];
}

int Graph::vertexCount() const
{
	return V.size();
}


Edge* Graph::addEdge(int vertex1, int vertex2, QString edgeLabel)
{
	while(V.size() <= vertex1 || V.size() <= vertex2)
	{
		addVertex();
	}

	return addEdge(V[vertex1], V[vertex2], edgeLabel);
}


/* TODO: this really needs to verify that these vertices are actually in
	this graph before adding the edge, both as a protection for public
	Graph APIs and to catch bugs in my own code...
*/
Edge* Graph::addEdge(Vertex* vertex1, Vertex* vertex2, QString edgeLabel)
{
	Edge* edge = new Edge(this, vertex1, vertex2, edgeLabel);
	E.append(edge);

	vertex1->E.append(edge);
	vertex2->E.append(edge);

	emit edgeAdded(edge);
	return edge;
}

void Graph::removeEdge(Edge* edge)
{
	emit edgeDeleting(edge);

	edge->v1->E.removeAll(edge);
	edge->v2->E.removeAll(edge);

	int index = E.indexOf(edge);
	E.removeAll(edge);

	emit edgeDeleted(index);
	emit edgeDeleted(edge->vertex1(), edge->vertex2());

}

void Graph::removeEdges()
{
	while(!E.isEmpty())
	{
		removeEdge(E.takeFirst());
	}
}

int Graph::edgeMultiplicity(int vertex1, int vertex2)
{
	return edgeMultiplicity(getVertex(vertex1), getVertex(vertex2));
}

int Graph::edgeMultiplicity(Vertex *vertex1, Vertex *vertex2)
{
	return getEdges(vertex1, vertex2).size();
}

void Graph::setEdgeMultiplicity(int vertex1, int vertex2, int count)
{
	setEdgeMultiplicity(getVertex(vertex1), getVertex(vertex2), count);
}

void Graph::setEdgeMultiplicity(Vertex *vertex1, Vertex *vertex2, int count)
{
	int mult = edgeMultiplicity(vertex1, vertex2);

	if(mult < count)
	{
		for(int i=mult; i<count; i++)
		{
			addEdge(vertex1, vertex2);
		}
	}
	else if(mult > count)
	{
		QList<Edge*> edges = getEdges(vertex1, vertex2);

		for(int i=count; i<mult; i++)
		{
			removeEdge(edges.takeLast());
		}
	}
}


Edge* Graph::getEdge(int index) const
{
	return E[index];
}

QList<Edge*> Graph::getEdgeSet() const
{
	return E;
}

bool Graph::hasEdge(int vertex1, int vertex2) const
{
	return hasEdge(V[vertex1], V[vertex2]);
}

/* TODO: This function and the one below it would be much faster if we took one of the given
 *       vertices and looped over all the (likely very few) edges connected to it, rather than
 *       looping over every single edge in the entire graph every time...
 */
bool Graph::hasEdge(Vertex* vertex1, Vertex* vertex2) const
{
	for(int e=0; e<E.size(); e++)
	{
		Edge* edge = E[e];

		if(   (edge->vertex1() == vertex1 && edge->vertex2() == vertex2)
		   || (edge->vertex1() == vertex2 && edge->vertex2() == vertex1))
		{
			return true;
		}
	}

	return false;
}


QList<Edge*> Graph::getEdges(int vertex1, int vertex2) const
{
	return getEdges(V[vertex1], V[vertex2]);
}

QList<Edge*> Graph::getEdges(Vertex* vertex1, Vertex* vertex2) const
{
	QList<Edge*> edges;

	for(int e=0; e<E.size(); e++)
	{
		Edge* edge = E[e];

		if(   (edge->vertex1() == vertex1 && edge->vertex2() == vertex2)
		   || (edge->vertex1() == vertex2 && edge->vertex2() == vertex1))
		{
			edges.append(edge);
		}
	}

	return edges;
}

int Graph::edgeCount() const
{
	return E.size();
}




Arc* Graph::addArc(int tail, int head, QString arcLabel)
{
	while(V.size() <= tail || V.size() <= head)
	{
		addVertex();
	}

	return addArc(V[tail], V[head], arcLabel);
}

Arc* Graph::addArc(Vertex* tail, Vertex* head, QString arcLabel)
{
	Arc* arc = new Arc(this, tail, head, arcLabel);
	A.append(arc);

	tail->O.append(arc);
	head->I.append(arc);

	emit arcAdded(arc);
	return arc;
}

void Graph::removeArc(Arc* arc)
{
	emit arcDeleting(arc);

	arc->tail()->O.removeAll(arc);
	arc->head()->I.removeAll(arc);

	int index = A.indexOf(arc);
	A.removeAll(arc);

	emit arcDeleted(index);
	emit arcDeleted(arc->tail(), arc->head());
}

void Graph::removeArcs()
{
	while(!A.isEmpty())
	{
		removeArc(A.takeFirst());
	}
}


int Graph::arcMultiplicity(int tail, int head)
{
	return arcMultiplicity(getVertex(tail), getVertex(head));
}

int Graph::arcMultiplicity(Vertex* tail, Vertex* head)
{
	return getArcs(tail, head).size();
}

void Graph::setArcMultiplicity(int tail, int head, int count)
{
	return setArcMultiplicity(getVertex(tail), getVertex(head), count);
}

void Graph::setArcMultiplicity(Vertex* tail, Vertex* head, int count)
{
	int mult = arcMultiplicity(tail, head);

	if(mult < count)
	{
		for(int i=mult; i<count; i++)
		{
			addArc(tail, head);
		}
	}
	else if(mult > count)
	{
		QList<Arc*> arcs = getArcs(tail, head);

		for(int i=count; i<mult; i++)
		{
			removeArc(arcs.takeLast());
		}
	}
}


Arc* Graph::getArc(int index) const
{
	return A[index];
}

void Graph::flipArc(Arc *arc)
{
	arc->tail()->O.removeAll(arc);
	arc->head()->I.removeAll(arc);

	Vertex* oldhead = arc->vhead;
	arc->vhead = arc->vtail;
	arc->vtail = oldhead;

	arc->tail()->O.append(arc);
	arc->head()->I.append(arc);

	emit arcFlipped(arc);
}

bool Graph::hasArc(int tail, int head) const
{
	return hasArc(V[tail], V[head]);
}

/* TODO: This function and the one below it would be much faster if we took one of the given
 *       vertices and looped over all the (likely very few) edges connected to it, rather than
 *       looping over every single edge in the entire graph every time...
 */
bool Graph::hasArc(Vertex* tail, Vertex* head) const
{
	for(int a=0; a<A.size(); a++)
	{
		Arc* arc = A[a];

		if(arc->tail() == tail && arc->head() == head)
		{
			return true;
		}
	}

	return false;
}


QList<Arc*> Graph::getArcSet() const
{
	return A;
}

QList<Arc*> Graph::getArcs(int tail, int head) const
{
	return getArcs(V[tail], V[head]);
}

QList<Arc*> Graph::getArcs(Vertex* tail, Vertex* head) const
{
	QList<Arc*> arcs;

	for(int a=0; a<A.size(); a++)
	{
		Arc* arc = A[a];

		if(arc->head() == head && arc->tail() == tail)
		{
			arcs.append(arc);
		}
	}

	return arcs;
}

int Graph::arcCount() const
{
	return A.size();
}

void Graph::clear()
{
	A.clear();
	E.clear();
	V.clear();

	emit graphCleared();
}



QList<QList<Vertex*> > Graph::getComponents() const
{
	QList<QList<Vertex*> > components;
	QList<Vertex*> vertices = getVertexSet();

	while(vertices.count() > 0)
	{
		QList<Vertex*> component;
		QList<Vertex*> stack;

		stack.append(vertices.takeFirst());

		while(!stack.isEmpty())
		{
			Vertex* v = stack.takeFirst();

			foreach(Vertex* nv, v->neighborhood())
			{
				if(vertices.contains(nv))
				{
					stack.append(nv);
					vertices.removeAll(nv);
				}
			}

			component.append(v);
		}

		components.append(component);
	}

	return components;
}


QString Graph::label() const
{
	return graphLabel;
}

void Graph::setLabel(QString newLabel)
{
	graphLabel = newLabel;

	emit labelChanged(newLabel);
}




bool Graph::isComplete()
{
	return (isCompleteGraph() || isCompleteDigraph());
}


bool Graph::isArcComplete()
{
	for(int v=0; v<vertexCount(); v++)
	{
		for(int w=v+1; w<vertexCount(); w++)
		{
			if(!hasArc(v, w) && !hasArc(w, v))
			{
				return false;
			}
		}
	}

	return true;
}


bool Graph::isEdgeComplete()
{
	for(int v=0; v<vertexCount(); v++)
	{
		for(int w=v+1; w<vertexCount(); w++)
		{
			if(!hasEdge(v, w))
			{
				return false;
			}
		}
	}

	return true;
}


bool Graph::isCompleteGraph()
{
	return (isPureGraph() && isEdgeComplete());
}


bool Graph::isCompleteDigraph()
{
	return (isPureDigraph() && isArcComplete());
}


bool Graph::isPure()
{
	return (isPureGraph() || isPureDigraph());
}


bool Graph::isPureGraph()
{
	return (arcCount() == 0);
}


bool Graph::isPureDigraph()
{
	return (edgeCount() == 0);
}


bool Graph::isEmpty()
{
	return (isPureGraph() && isPureDigraph());
}


bool Graph::isNull()
{
	return (vertexCount() == 0);
}


// TODO: Should a null graph be considered trivial? If not, then if "nontrivial" == !isTrival, then null is nontrivial...
bool Graph::isTrivial()
{
	return (vertexCount() == 1);
}


Graph* Graph::getCompetitionGraph() const
{
	return CompetitionGraph::of(this);
}

Graph* Graph::getComplementGraph() const
{
	return ComplementGraph::of(this);
}


Graph* Graph::getConverseGraph() const
{
	return ConverseGraph::of(this);
}

Graph* Graph::getDominationGraph() const
{
	return DominationGraph::of(this);
}

Graph* Graph::getUnderlyingGraph() const
{
	return UnderlyingGraph::of(this);
}
