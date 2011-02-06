#include <QStyle>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "graphcore/representations/intervalgraph.h"
#include "intervalgraphdraw.h"

#include "intervalgraphwindow.h"

IntervalGraphWindow::IntervalGraphWindow(Graph* graph, GraphShopWindow *parent): GraphWindow(parent)
{
	_graph = new IntervalGraph(graph);

	updateTitle();
}

IntervalGraphWindow::IntervalGraphWindow(IntervalGraph *graph, GraphShopWindow *parent): GraphWindow(parent)
{
	_graph = graph;

	updateTitle();
}

void IntervalGraphWindow::_build()
{
	_graph->build();

	_draw = new IntervalGraphDraw(_graph);
	widget()->layout()->addWidget(_draw);


	_toolbar->insertAction(0, _draw->exportAction());
	_toolbar->addSeparator();


	QAction* zoomIn = new QAction("+", _toolbar);
	_toolbar->addAction(zoomIn);

	QAction* zoomOut = new QAction("-", _toolbar);
	_toolbar->addAction(zoomOut);

	QAction* zoomAll = new QAction("A", _toolbar);
	_toolbar->addAction(zoomAll);

	_toolbar->addSeparator();

	QAction* addInterval = new QAction("I+", _toolbar);
	_toolbar->addAction(addInterval);


	connect(zoomIn,        SIGNAL(triggered()), SLOT(zoomIn()));
	connect(zoomOut,       SIGNAL(triggered()), SLOT(zoomOut()));
	connect(zoomAll,       SIGNAL(triggered()), SLOT(zoomAll()));
	connect(addInterval,     SIGNAL(triggered()), SLOT(addInterval()));

	connect(_graph->source(), SIGNAL(labelChanged(QString)), SLOT(updateTitle()));
}

/*
void IntervalGraphWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
	{
		draw->setMode(TournamentDraw::SELECT);
	}
}
*/


void IntervalGraphWindow::zoomIn()
{
	_draw->scale(1.2, 1.2);
}

void IntervalGraphWindow::zoomOut()
{
	_draw->scale(1/1.2, 1/1.2);
}

void IntervalGraphWindow::zoomAll()
{
	_draw->viewAll();
}

void IntervalGraphWindow::addInterval()
{
	_draw->addInterval();
}

void IntervalGraphWindow::updateTitle()
{
	setTitle("Interval Graph: " + _graph->source()->label());
}
