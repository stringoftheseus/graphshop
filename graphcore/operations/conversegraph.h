#ifndef conversegraph_h
#define conversegraph_h

#include "graphoperation.h"

class ConverseGraph : public GraphOperation
{
	Q_OBJECT

public:
	ConverseGraph(Graph const* source);
	static QScriptValue sConverseGraph(QScriptContext* context, QScriptEngine* engine);

	static Graph* of(Graph const* source);

private:
	virtual void _build();

private slots:
	virtual void arcAdded(Arc*);
	virtual void arcFlipped(Arc*);
	virtual void arcDeleted(int);

	virtual void edgeAdded(Edge*);
	virtual void edgeDeleted(int);

	virtual void vertexAdded(Vertex*);
	virtual void vertexDeleted(int);

	virtual void graphCleared();

	virtual void updateLabel();


	/* TODO: update the slots to use deleted(int) instead of deleted(obj) (which doens't exist anymore)
			 also, add a graphcleared slot, implement that, and make sure we're not implmenting any of
			 these that we don't actually need.

			 Also, do all that for domination graph also...
	*/


};

Q_DECLARE_METATYPE(ConverseGraph*)

#endif // ConverseGraph_H
