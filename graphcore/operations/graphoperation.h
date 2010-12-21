#ifndef GRAPHOPERATION_H
#define GRAPHOPERATION_H

#include "graphcore/_graphcore_c.h"
#include "graphcore/graph.h"

class GraphOperation : public Graph
{
	Q_OBJECT

public:
	GraphOperation(Graph const* source);

	Q_INVOKABLE void setSource(Graph const* source, bool autobuild = false);
	Q_INVOKABLE void build();

protected:
	Graph const* _source;

	void defaultBuild();
	void synchronizeVertices();

protected slots:
	virtual void arcAdded(Arc*);
	virtual void arcFlipped(Arc*);
	virtual void arcDeleting(Arc*);
	virtual void arcDeleted(int);
	virtual void arcDeleted(Vertex*, Vertex*);

	virtual void edgeAdded(Edge*);
	virtual void edgeDeleting(Edge*);
	virtual void edgeDeleted(int);
	virtual void edgeDeleted(Vertex*, Vertex*);

	virtual void vertexAdded(Vertex*);
	virtual void vertexDeleting(Vertex*);
	virtual void vertexDeleted(int);

	virtual void graphCleared();

	virtual void updateLabel();

private:
	void connectGraphSignals();

	bool _built;
	virtual void _build() = 0;
};

#endif //GRAPHOPERATION_H
