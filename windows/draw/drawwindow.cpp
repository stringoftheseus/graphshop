#include <QStyle>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

#include "layout/randomlayout.h"
#include "layout/circlelayout.h"
#include "layout/gravitylayout.h"

#include "drawwindow.h"

DrawWindow::DrawWindow(Graph *graph, GraphShopWindow *parent): GraphWindow(parent), _graph(graph)
{
	draw = 0;

	updateTitle();
}

void DrawWindow::_build()
{
	_graph->build();


	QActionGroup* modeGroup = new QActionGroup(this);
	connect(modeGroup, SIGNAL(triggered(QAction*)), SLOT(setMode(QAction*)));

	QAction* modeSelect = new QAction("Select", this);
	modeSelect->setCheckable(true);
	modeSelect->setChecked(true);
	modeSelect->setData(GraphDraw::SELECT);
	modeGroup->addAction(modeSelect);

	QAction* modePan = new QAction("Pan", this);
	modePan->setCheckable(true);
	modePan->setData(GraphDraw::PAN);
	modeGroup->addAction(modePan);

	QAction* modeAddVertex = new QAction("Add Vertex", this);
	modeAddVertex->setCheckable(true);
	modeAddVertex->setData(GraphDraw::ADD_VERTEX);
	modeGroup->addAction(modeAddVertex);

	QAction* modeAddEdge = new QAction("Add Edge", this);
	modeAddEdge->setCheckable(true);
	modeAddEdge->setData(GraphDraw::ADD_EDGE);
	modeGroup->addAction(modeAddEdge);

	QAction* modeAddArc = new QAction("Add Arc", this);
	modeAddArc->setCheckable(true);
	modeAddArc->setData(GraphDraw::ADD_ARC);
	modeGroup->addAction(modeAddArc);

	QMenu* modeMenu = new QMenu(this);
	modeMenu->addActions(modeGroup->actions());

	QToolButton* modeButton = new QToolButton();
	modeButton->setText("~");
	modeButton->setMenu(modeMenu);
	modeButton->setPopupMode(QToolButton::InstantPopup);

	_toolbar->addWidget(modeButton);



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


	QAction* zoomIn = new QAction("+", _toolbar);
	_toolbar->addAction(zoomIn);

	QAction* zoomOut = new QAction("-", _toolbar);
	_toolbar->addAction(zoomOut);

	QAction* zoomStd = new QAction("0", _toolbar);
	_toolbar->addAction(zoomStd);

	QAction* zoomAll = new QAction("A", _toolbar);
	_toolbar->addAction(zoomAll);

	_toolbar->addSeparator();

	QAction* rotateCW = new QAction(">", _toolbar);
	_toolbar->addAction(rotateCW);

	QAction* rotateCCW = new QAction("<", _toolbar);
	_toolbar->addAction(rotateCCW);


	connect(zoomIn,        SIGNAL(triggered()), SLOT(zoomIn()));
	connect(zoomOut,       SIGNAL(triggered()), SLOT(zoomOut()));
	connect(zoomStd,       SIGNAL(triggered()), SLOT(zoomStd()));
	connect(zoomAll,       SIGNAL(triggered()), SLOT(zoomAll()));
	connect(rotateCW,      SIGNAL(triggered()), SLOT(rotateCW()));
	connect(rotateCCW,     SIGNAL(triggered()), SLOT(rotateCCW()));
	connect(layoutRandom,  SIGNAL(triggered()), SLOT(layoutRandom()));
	connect(layoutCircle,  SIGNAL(triggered()), SLOT(layoutCircle()));
	connect(layoutGravity, SIGNAL(triggered()), SLOT(layoutGravity()));

	draw = new GraphDraw(_graph);
	widget()->layout()->addWidget(draw);

	connect(_graph, SIGNAL(labelChanged(QString)), SLOT(updateTitle()));
}


void DrawWindow::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	if(draw)
	{
		draw->updateRect();
	}
}

/*
void DrawWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Escape)
	{
		draw->setMode(GraphDraw::SELECT);
	}
}
*/


void DrawWindow::zoomIn()
{
	draw->scale(1.2, 1.2);
}

void DrawWindow::zoomOut()
{
	draw->scale(1/1.2, 1/1.2);
}

void DrawWindow::zoomStd()
{
	draw->resetView();
}

void DrawWindow::zoomAll()
{
	draw->viewAll();
}

void DrawWindow::rotateCW()
{
	/* TODO: The ideal effect here would be to rotate the graph, but keep the vertices "in place" so
			 that the numberic labels on each still print right side up. We can do this by calling
			 setRotation on each vertexdraw to be this much more in the opposite direction than it
			 currently is. The tricky part here might be making sure that all new vertexdraws added
			 after this happens also get rotated accordingly.

			 The other option would be to just not use the built-in rotate functions at all, but
			 provide a custom layout-like system that does a rotation algorithm itself by moving
			 each vertex to a new position defined by the requested rotation. This might actually be
			 simpler in the long run even if it takes way more work now...

			 Also, the latter would allow me to rotate only the selected vertices, not the entire view,
			 which would be really awesome!
	*/

	draw->rotate(22.5);
}

void DrawWindow::rotateCCW()
{
	draw->rotate(-22.5);
}

void DrawWindow::layoutRandom()
{
	draw->doLayout(new RandomLayout());
}

void DrawWindow::layoutCircle()
{
	draw->doLayout(new CircleLayout(), 2);
}

void DrawWindow::layoutGravity()
{
	draw->doLayout(new GravityLayout(), 2);
}

void DrawWindow::updateTitle()
{
	setTitle("Drawing: " + _graph->label());
}

void DrawWindow::setMode(QAction *action)
{
	draw->setMode(static_cast<GraphDraw::InteractionMode>(action->data().toInt()));
}
