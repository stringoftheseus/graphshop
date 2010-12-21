#ifndef GRAPHLINK_H
#define GRAPHLINK_H

#include "graphcore/_graphcore_c.h"
#include "graphcore/graph.h"

class GraphRepresentation : public QObject
{
	Q_OBJECT

public:
	GraphRepresentation(Graph* source);

	Q_INVOKABLE Graph* source();
	Q_INVOKABLE void setSource(Graph* source, bool autobuild = false);
	Q_INVOKABLE void build();

	Q_INVOKABLE bool valid();

signals:
	void validityChanged(bool);

protected:
	Graph* _source;

	void defaultBuild();

	void validate();
	void setValid(bool);
	virtual bool _valid() = 0;

protected slots:
	virtual void _arcAdded(Arc*);
	virtual void _arcFlipped(Arc*);
	virtual void _arcDeleting(Arc*);
	virtual void _arcDeleted(int);
	virtual void _arcDeleted(Vertex*, Vertex*);

	virtual void _edgeAdded(Edge*);
	virtual void _edgeDeleting(Edge*);
	virtual void _edgeDeleted(int);
	virtual void _edgeDeleted(Vertex*, Vertex*);

	virtual void _vertexAdded(Vertex*);
	virtual void _vertexDeleting(Vertex*);
	virtual void _vertexDeleted(int);

	virtual void _graphCleared();

private:
	void connectGraphSignals();

	bool _built;
	virtual void _build() = 0;

	bool _isvalid;
};

#endif //GRAPHLINK_H
