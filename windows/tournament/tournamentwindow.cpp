#include <QStyle>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "tournamentwindow.h"

TournamentWindow::TournamentWindow(Graph* graph, GraphShopWindow *parent): GraphWindow(parent)
{
	_tournament = new Tournament(graph);

	updateTitle();
}

TournamentWindow::TournamentWindow(Tournament *tournament, GraphShopWindow *parent): GraphWindow(parent)
{
	_tournament = tournament;

	updateTitle();
}

void TournamentWindow::_build()
{
	_tournament->build();

	draw = new TournamentDraw(_tournament);
	widget()->layout()->addWidget(draw);

	_toolbar->insertAction(0, draw->exportAction());
	_toolbar->addSeparator();

	QAction* zoomIn = new QAction(QIcon("img/zoom_in.png"), "Zoom In", _toolbar);
	_toolbar->addAction(zoomIn);

	QAction* zoomOut = new QAction(QIcon("img/zoom_out.png"), "Zoom Out", _toolbar);
	_toolbar->addAction(zoomOut);

	QAction* zoomAll = new QAction(QIcon("img/view_all.png"), "Zoom Fit", _toolbar);
	_toolbar->addAction(zoomAll);

	_toolbar->addSeparator();

	QAction* addVertex = new QAction(QIcon("img/add_vertex_plus.png"), "Add Vertex", _toolbar);
	_toolbar->addAction(addVertex);


	connect(zoomIn,        SIGNAL(triggered()), SLOT(zoomIn()));
	connect(zoomOut,       SIGNAL(triggered()), SLOT(zoomOut()));
	connect(zoomAll,       SIGNAL(triggered()), SLOT(zoomAll()));
	connect(addVertex,     SIGNAL(triggered()), SLOT(addVertex()));


	connect(_tournament->source(), SIGNAL(labelChanged(QString)), SLOT(updateTitle()));
}

/*
void TournamentWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
	{
		draw->setMode(TournamentDraw::SELECT);
	}
}
*/


void TournamentWindow::zoomIn()
{
	draw->scale(1.2, 1.2);
}

void TournamentWindow::zoomOut()
{
	draw->scale(1/1.2, 1/1.2);
}

void TournamentWindow::zoomAll()
{
	draw->viewAll();
}

void TournamentWindow::addVertex()
{
	draw->addVertex();
}

void TournamentWindow::updateTitle()
{
	setTitle("Tournament: " + _tournament->source()->label());
}
