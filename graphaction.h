#ifndef GRAPHACTION_H
#define GRAPHACTION_H

#include <QAction>

#include "graphcore/graph.h"

class GraphAction : public QAction
{
	Q_OBJECT

public:
	explicit GraphAction(Graph* graph, bool dots = false, QObject *parent = 0);

protected:
	Graph* graph;
	bool dots;

signals:
	void triggered(Graph*);

protected slots:
	void processTrigger();
	void updateText(QString text);
};

#endif // GRAPHACTION_H
