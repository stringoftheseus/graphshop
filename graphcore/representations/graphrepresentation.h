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

	void ignoreSignals(bool);
	bool signalsIgnored();

	void defaultBuild();

	void validate();
	void setValid(bool);
	virtual bool _valid() = 0;

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

protected slots:
	void _s_arcAdded(Arc*);
	void _s_arcFlipped(Arc*);
	void _s_arcDeleting(Arc*);
	void _s_arcDeleted(int);
	void _s_arcDeleted(Vertex*, Vertex*);

	void _s_edgeAdded(Edge*);
	void _s_edgeDeleting(Edge*);
	void _s_edgeDeleted(int);
	void _s_edgeDeleted(Vertex*, Vertex*);

	void _s_vertexAdded(Vertex*);
	void _s_vertexDeleting(Vertex*);
	void _s_vertexDeleted(int);

	void _s_graphCleared();

private:
	void connectGraphSignals();

	bool _built;
	virtual void _build() = 0;

	bool _isvalid;
	bool _ignoreSignals;
};

#endif //GRAPHLINK_H
