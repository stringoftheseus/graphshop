#ifndef COMPETITIONGRAPH_H
#define COMPETITIONGRAPH_H

#include "graphoperation.h"

class CompetitionGraph : public GraphOperation
{
	Q_OBJECT

public:
	CompetitionGraph(Graph const* source);
	static QScriptValue sCompetitionGraph(QScriptContext* context, QScriptEngine* engine);

	Q_INVOKABLE static Graph* of(Graph const* source);
	Q_INVOKABLE static bool isCompetitivePair(Graph const* graph, int vertex1, int vertex2);
	Q_INVOKABLE static bool isCompetitivePair(Graph const* graph, Vertex* vertex1, Vertex* vertex2);

protected:
	virtual void _build();

	void rebuild();

protected slots:
	virtual void arcAdded(Arc*);
	virtual void arcFlipped(Arc*);
	virtual void arcDeleted(int);

	virtual void vertexAdded(Vertex*);
	virtual void vertexDeleted(int);

	virtual void graphCleared();

	virtual void updateLabel();
};

Q_DECLARE_METATYPE(CompetitionGraph*)

#endif // COMPETITIONGRAPH_H
