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


/*
	QActionGroup* modeGroup = new QActionGroup(this);
	connect(modeGroup, SIGNAL(triggered(QAction*)), SLOT(setMode(QAction*)));

	QAction* modeSelect = new QAction("Select", this);
	modeSelect->setCheckable(true);
	modeSelect->setChecked(true);
	modeSelect->setData(TournamentDraw::SELECT);
	modeGroup->addAction(modeSelect);

	QAction* modePan = new QAction("Pan", this);
	modePan->setCheckable(true);
	modePan->setData(TournamentDraw::PAN);
	modeGroup->addAction(modePan);

	QAction* modeAddVertex = new QAction("Add Vertex", this);
	modeAddVertex->setCheckable(true);
	modeAddVertex->setData(TournamentDraw::ADD_VERTEX);
	modeGroup->addAction(modeAddVertex);

	QAction* modeAddEdge = new QAction("Add Edge", this);
	modeAddEdge->setCheckable(true);
	modeAddEdge->setData(TournamentDraw::ADD_EDGE);
	modeGroup->addAction(modeAddEdge);

	QAction* modeAddArc = new QAction("Add Arc", this);
	modeAddArc->setCheckable(true);
	modeAddArc->setData(TournamentDraw::ADD_ARC);
	modeGroup->addAction(modeAddArc);

	QMenu* modeMenu = new QMenu(this);
	modeMenu->addActions(modeGroup->actions());

	QToolButton* modeButton = new QToolButton();
	modeButton->setText("~");
	modeButton->setMenu(modeMenu);
	modeButton->setPopupMode(QToolButton::InstantPopup);

	_toolbar->addWidget(modeButton);
*/

/*
	QToolButton* layoutButton = new QToolButton();
	layoutButton->setText("#");

	QMenu* layoutMenu = new QMenu(this);
	layoutButton->setMenu(layoutMenu);
	layoutButton->setPopupMode(QToolButton::InstantPopup);

	QAction* layoutRandom = new QAction("Random", this);
	layoutRandom->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_R);
	layoutMenu->addAction(layoutRandom);

	QAction* layoutCircle = new QAction("Circle", this);
	layoutCircle->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_C);
	layoutMenu->addAction(layoutCircle);

	QAction* layoutGravity = new QAction("Gravity", this);
	layoutGravity->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_G);
	layoutMenu->addAction(layoutGravity);

	_toolbar->addWidget(layoutButton);


	_toolbar->addSeparator();
*/

	QAction* zoomIn = new QAction("+", _toolbar);
	_toolbar->addAction(zoomIn);

	QAction* zoomOut = new QAction("-", _toolbar);
	_toolbar->addAction(zoomOut);

	QAction* zoomAll = new QAction("A", _toolbar);
	_toolbar->addAction(zoomAll);

	_toolbar->addSeparator();

	QAction* addInterval = new QAction("I+", _toolbar);
	_toolbar->addAction(addInterval);



/*
	_toolbar->addSeparator();

	QAction* rotateCW = new QAction(">", _toolbar);
	_toolbar->addAction(rotateCW);

	QAction* rotateCCW = new QAction("<", _toolbar);
	_toolbar->addAction(rotateCCW);
*/

	connect(zoomIn,        SIGNAL(triggered()), SLOT(zoomIn()));
	connect(zoomOut,       SIGNAL(triggered()), SLOT(zoomOut()));
	connect(zoomAll,       SIGNAL(triggered()), SLOT(zoomAll()));
	connect(addInterval,     SIGNAL(triggered()), SLOT(addInterval()));

/*	connect(rotateCW,      SIGNAL(triggered()), SLOT(rotateCW()));
	connect(rotateCCW,     SIGNAL(triggered()), SLOT(rotateCCW()));
	connect(layoutRandom,  SIGNAL(triggered()), SLOT(layoutRandom()));
	connect(layoutCircle,  SIGNAL(triggered()), SLOT(layoutCircle()));
	connect(layoutGravity, SIGNAL(triggered()), SLOT(layoutGravity()));
*/

	_draw = new IntervalGraphDraw(_graph);
	widget()->layout()->addWidget(_draw);

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
