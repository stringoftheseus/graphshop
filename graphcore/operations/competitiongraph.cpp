#include "graphcore/_graphcore_h.h"

#include "competitiongraph.h"

CompetitionGraph::CompetitionGraph(const Graph *source): GraphOperation(source)
{
	updateLabel();
}

QScriptValue CompetitionGraph::sCompetitionGraph(QScriptContext *context, QScriptEngine *engine)
{
	return engine->newQObject(new CompetitionGraph(qobject_cast<Graph*>(context->argument(0).toQObject())));
}

Graph* CompetitionGraph::of(Graph const* source)
{
	CompetitionGraph com(source);
	com.build();

	return com.clone();
}

void CompetitionGraph::_build()
{
	synchronizeVertices();

	for(int u=0; u<_source->vertexCount(); u++)
	{
		for(int v = u+1; v<_source->vertexCount(); v++)
		{
			if(isCompetitivePair(_source, u, v))
			{
				addEdge(u, v);
			}
		}
	}

	updateLabel();
}

void CompetitionGraph::rebuild()
{
	removeEdges();
	_build();
}

bool CompetitionGraph::isCompetitivePair(Graph const* graph, int vertex1, int vertex2)
{
	return isCompetitivePair(graph, graph->getVertex(vertex1), graph->getVertex(vertex2));
}

bool CompetitionGraph::isCompetitivePair(Graph const* graph, Vertex* vertex1, Vertex* vertex2)
{
	QList<Vertex*> vs1 = vertex1->outNeighborhood();
	QList<Vertex*> vs2 = vertex2->outNeighborhood();

	return (QSet<Vertex*>::fromList(vs1).intersect(QSet<Vertex*>::fromList(vs2)).count() > 0);
}


void CompetitionGraph::arcAdded(Arc *arc)
{
	rebuild();
}

void CompetitionGraph::arcFlipped(Arc *arc)
{
	rebuild();
}

void CompetitionGraph::arcDeleted(int index)
{
	rebuild();
}

void CompetitionGraph::vertexAdded(Vertex *vertex)
{
	rebuild();
}

void CompetitionGraph::vertexDeleted(int index)
{
	rebuild();
}

void CompetitionGraph::graphCleared()
{
	clear();
}

void CompetitionGraph::updateLabel()
{
	setLabel("compet(" +_source->label()+")");
}
