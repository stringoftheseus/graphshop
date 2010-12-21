#include "graphaction.h"

GraphAction::GraphAction(Graph* graph, QObject *parent): QAction(parent), graph(graph)
{
	updateText(graph->label());

	connect(graph, SIGNAL(labelChanged(QString)), SLOT(updateText(QString)));
	connect(this, SIGNAL(triggered()), SLOT(processTrigger()));
}

void GraphAction::updateText(QString text)
{
	setText(text);
}

void GraphAction::processTrigger()
{
	emit triggered(graph);
}
