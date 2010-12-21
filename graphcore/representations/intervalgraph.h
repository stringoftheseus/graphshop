#ifndef INTERVALGRAPH_H
#define INTERVALGRAPH_H

#include <QObject>
#include "graphcore/_graphcore_c.h"

#include "graphrepresentation.h"

class Interval;

class IntervalGraph : public GraphRepresentation
{
    Q_OBJECT

public:
	explicit IntervalGraph(QObject *parent);
	explicit IntervalGraph(Graph* source);

	int intervalCount();
	Interval* getInterval(int index);

	void addInterval(double left, double right);
	void deleteInterval(Interval* interval);

signals:
	void intervalAdded(Interval*);
	void intervalDeleting(Interval*);
	void intervalMoved(Interval*);

	void cleared();

protected:
	virtual bool _valid();
	virtual void _build();

	void addInterval(double left, double right, Vertex* vertex);

	bool _working;
	double _endpoint;
	QHash<Vertex*, Interval*> _intervals;

protected slots:
	virtual void _intervalMoved(Interval*);

	virtual void _edgeAdded(Edge*);
	virtual void _edgeDeleted(int);

	virtual void _vertexAdded(Vertex*);
	virtual void _vertexDeleting(Vertex*);

	virtual void _graphCleared();
};

#endif // INTERVALGRAPH_H
