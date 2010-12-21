#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "graphrepresentation.h"

class Tournament : public GraphRepresentation
{
    Q_OBJECT

public:
	explicit Tournament(QObject* parent);
	explicit Tournament(Graph* source);

	int addVertex();
	void removeVertex(int index);
	int order();

	bool hasArc(int a, int b);
	void setArc(int a, int b);
	void flipArc(int a, int b);

	QList<int> scoreSequence();

signals:
	void vertexAdded(int);
	void vertexDeleted(int);
	void cleared();

	void arcFlipped(int, int);

protected:
	bool _valid();

protected slots:
	void _arcAdded(Arc*);
	void _arcFlipped(Arc*);
	void _arcDeleted(int);

	void _vertexAdded(Vertex*);
	void _vertexDeleting(Vertex*);
	void _vertexDeleted(int);

	void _graphCleared();

private:
	virtual void _build();

};

#endif // TOURNAMENT_H
