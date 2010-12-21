#ifndef GRAPHACTION_H
#define GRAPHACTION_H

#include <QAction>

#include "graphcore/graph.h"

class GraphAction : public QAction
{
	Q_OBJECT

public:
	explicit GraphAction(Graph* graph, QObject *parent = 0);

protected:
	Graph* graph;

signals:
	void triggered(Graph*);

protected slots:
	void processTrigger();
	void updateText(QString text);
};

#endif // GRAPHACTION_H
