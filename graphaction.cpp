#include "graphaction.h"

GraphAction::GraphAction(Graph* graph, bool dots, QObject *parent): QAction(parent), graph(graph), dots(dots)
{
	updateText(graph->label());

	connect(graph, SIGNAL(labelChanged(QString)), SLOT(updateText(QString)));
	connect(this, SIGNAL(triggered()), SLOT(processTrigger()));
}

void GraphAction::updateText(QString text)
{
	setText(text + (dots ? "..." : ""));
}

void GraphAction::processTrigger()
{
	emit triggered(graph);
}
