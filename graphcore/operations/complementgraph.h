#ifndef COMPLEMENTGRAPH_H
#define COMPLEMENTGRAPH_H

#include "graphoperation.h"

class ComplementGraph : public GraphOperation
{
    Q_OBJECT

public:
	explicit ComplementGraph(Graph const* source);
	static QScriptValue sComplementGraph(QScriptContext* context, QScriptEngine* engine);

	Q_INVOKABLE static Graph* of(Graph const* source);

protected:
	virtual void _build();
	virtual void updateEdgeCandidate(int vertex1, int vertex2);
	virtual void updateEdgeCandidate(Vertex* vertex1, Vertex* vertex2);

protected slots:
	virtual void edgeAdded(Edge*);
	virtual void edgeDeleted(Vertex*, Vertex*);

	virtual void vertexAdded(Vertex*);
	virtual void vertexDeleted(int);

	virtual void graphCleared();

	virtual void updateLabel();

};

Q_DECLARE_METATYPE(ComplementGraph*)

#endif // UNDERLYINGGRAPH_H
