#include <QStyle>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "layout/randomlayout.h"
#include "layout/circlelayout.h"
#include "layout/gravitylayout.h"
#include "layout/rotatelayout.h"

#include "drawwindow.h"

DrawWindow::DrawWindow(Graph *graph, GraphShopWindow *parent): GraphWindow(parent), _graph(graph)
{
	_draw = 0;

	updateTitle();
}

GraphDraw* DrawWindow::draw()
{
	return _draw;
}

void DrawWindow::_build()
{
	_graph->build();

	_draw = new GraphDraw(_graph);
	widget()->layout()->addWidget(_draw);

	_toolbar->insertAction(0, _draw->exportAction());
	_toolbar->addSeparator();


	QActionGroup* modeGroup = new QActionGroup(this);
	connect(modeGroup, SIGNAL(triggered(QAction*)), SLOT(setMode(QAction*)));

	QAction* modeSelect = new QAction(QIcon("img/select.png"), "Select", this);
	modeSelect->setCheckable(true);
	modeSelect->setChecked(true);
	modeSelect->setData(GraphDraw::SELECT);
	modeGroup->addAction(modeSelect);

	QAction* modePan = new QAction(QIcon("img/pan.png"), "Pan", this);
	modePan->setCheckable(true);
	modePan->setData(GraphDraw::PAN);
	modeGroup->addAction(modePan);

	QAction* modeAddVertex = new QAction(QIcon("img/add_vertex_plus.png"), "Add Vertex", this);
	modeAddVertex->setCheckable(true);
	modeAddVertex->setData(GraphDraw::ADD_VERTEX);
	modeGroup->addAction(modeAddVertex);

	QAction* modeAddEdge = new QAction(QIcon("img/add_edge_plus.png"), "Add Edge", this);
	modeAddEdge->setCheckable(true);
	modeAddEdge->setData(GraphDraw::ADD_EDGE);
	modeGroup->addAction(modeAddEdge);

	QAction* modeAddArc = new QAction(QIcon("img/add_arc_plus.png"), "Add Arc", this);
	modeAddArc->setCheckable(true);
	modeAddArc->setData(GraphDraw::ADD_ARC);
	modeGroup->addAction(modeAddArc);

	QMenu* modeMenu = new QMenu(this);
	modeMenu->addActions(modeGroup->actions());

	_modeButton = new QToolButton();
	_modeButton->setMenu(modeMenu);
	_modeButton->setPopupMode(QToolButton::InstantPopup);

	_toolbar->addWidget(_modeButton);

	modeSelect->trigger();



	QActionGroup* layoutGroup = new QActionGroup(this);
	connect(layoutGroup, SIGNAL(triggered(QAction*)), SLOT(layoutChange(QAction*)));

	QAction* layoutRandom = new QAction(QIcon("img/layout_random.png"), "Random Layout", this);
	//layoutRandom->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_R);
	layoutRandom->setCheckable(true);
	layoutGroup->addAction(layoutRandom);

	QAction* layoutCircle = new QAction(QIcon("img/layout_circle.png"), "Circle Layout", this);
	//layoutCircle->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_C);
	layoutCircle->setCheckable(true);
	layoutGroup->addAction(layoutCircle);

	QAction* layoutGravity = new QAction(QIcon("img/layout_gravity.png"), "Gravity Layout", this);
	//layoutGravity->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_G);
	layoutGravity->setCheckable(true);
	layoutGroup->addAction(layoutGravity);


	QMenu* layoutMenu = new QMenu(this);
	layoutMenu->addActions(layoutGroup->actions());

	_layoutButton = new QToolButton();
	_layoutButton->setMenu(layoutMenu);
	_layoutButton->setText("#");
	_layoutButton->setPopupMode(QToolButton::MenuButtonPopup);

	_toolbar->addWidget(_layoutButton);
	layoutRandom->trigger();

	_toolbar->addSeparator();



	QAction* zoomIn = new QAction(QIcon("img/zoom_in.png"), "Zoom In", _toolbar);
	_toolbar->addAction(zoomIn);

	QAction* zoomOut = new QAction(QIcon("img/zoom_out.png"), "Zoom Out", _toolbar);
	_toolbar->addAction(zoomOut);

/*	QAction* zoomStd = new QAction("0", _toolbar);
	_toolbar->addAction(zoomStd);
*/
	QAction* zoomAll = new QAction(QIcon("img/view_all.png"), "Zoom Fit", _toolbar);
	_toolbar->addAction(zoomAll);

	_toolbar->addSeparator();

	QAction* rotateCW = new QAction(QIcon("img/rotate_right.png"), "Rotate Clockwise", _toolbar);
	_toolbar->addAction(rotateCW);

	QAction* rotateCCW = new QAction(QIcon("img/rotate_left.png"), "Rotate Counterclockwise", _toolbar);
	_toolbar->addAction(rotateCCW);


	connect(zoomIn,        SIGNAL(triggered()), SLOT(zoomIn()));
	connect(zoomOut,       SIGNAL(triggered()), SLOT(zoomOut()));
//	connect(zoomStd,       SIGNAL(triggered()), SLOT(zoomStd()));
	connect(zoomAll,       SIGNAL(triggered()), SLOT(zoomAll()));
	connect(rotateCW,      SIGNAL(triggered()), SLOT(rotateCW()));
	connect(rotateCCW,     SIGNAL(triggered()), SLOT(rotateCCW()));
	connect(layoutRandom,  SIGNAL(triggered()), SLOT(layoutRandom()));
	connect(layoutCircle,  SIGNAL(triggered()), SLOT(layoutCircle()));
	connect(layoutGravity, SIGNAL(triggered()), SLOT(layoutGravity()));

	connect(_graph, SIGNAL(labelChanged(QString)), SLOT(updateTitle()));
}


void DrawWindow::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if(_draw)
	{
		_draw->updateRect();
	}
}

/*
void DrawWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
	{
		_draw->setMode(GraphDraw::SELECT);
	}
}
*/


void DrawWindow::zoomIn()
{
	_draw->scale(1.2, 1.2);
}

void DrawWindow::zoomOut()
{
	_draw->scale(1/1.2, 1/1.2);
}

void DrawWindow::zoomStd()
{
	_draw->resetView();
}

void DrawWindow::zoomAll()
{
	_draw->viewAll();
}

void DrawWindow::rotateCW()
{
	_draw->doLayout(new RotateLayout(22.5), 2);
}

void DrawWindow::rotateCCW()
{
	_draw->doLayout(new RotateLayout(-22.5), 2);
}

void DrawWindow::layoutChange(QAction *action)
{
	_layoutButton->setIcon(action->icon());
	_layoutButton->setText(action->text());

	_layoutButton->disconnect();
	connect(_layoutButton, SIGNAL(clicked()), action, SLOT(trigger()));
}

void DrawWindow::layoutRandom()
{
	_draw->doLayout(new RandomLayout());
}

void DrawWindow::layoutCircle()
{
	_draw->doLayout(new CircleLayout(), 2);
}

void DrawWindow::layoutGravity()
{
	_draw->doLayout(new GravityLayout(), 2);
}

void DrawWindow::updateTitle()
{
	setTitle("Drawing: " + _graph->label());
}

void DrawWindow::setMode(QAction *action)
{
	_modeButton->setIcon(action->icon());
	_modeButton->setToolTip("Interaction Mode: " + action->text());
	_draw->setMode(static_cast<GraphDraw::InteractionMode>(action->data().toInt()));
}
