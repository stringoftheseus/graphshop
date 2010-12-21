#ifndef UNDERLYINGGRAPH_H
#define UNDERLYINGGRAPH_H

#include "graphoperation.h"

class UnderlyingGraph : public GraphOperation
{
    Q_OBJECT

public:
	explicit UnderlyingGraph(Graph const* source);
	static QScriptValue sUnderlyingGraph(QScriptContext* context, QScriptEngine* engine);

	Q_INVOKABLE static Graph* of(Graph const* source);

protected:
	QHash<Edge*, Edge*> _edges;
	QHash<Arc*, Edge*> _arcs;

	virtual void _build();

protected slots:
	virtual void arcAdded(Arc*);
	virtual void arcDeleting(Arc*);

	virtual void edgeAdded(Edge*);
	virtual void edgeDeleting(Edge*);

	virtual void vertexAdded(Vertex*);
	virtual void vertexDeleted(int);

	virtual void graphCleared();

	virtual void updateLabel();

};

Q_DECLARE_METATYPE(UnderlyingGraph*)

#endif // UNDERLYINGGRAPH_H
