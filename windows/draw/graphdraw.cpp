#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QtGui>

#include "layout/drawlayout.h"

#include "graphdraw.h"
#include "vertexdraw.h"
#include "edgedraw.h"
#include "arcdraw.h"

GraphDraw::GraphDraw(Graph *graph): graph(graph)
{
	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setBackgroundBrush(palette().background());

	//scene->setSceneRect(-200, -200, 200, 200);
	//scene->setSceneRect(rect());
	setScene(scene);

	setViewportUpdateMode(FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorViewCenter);
	//setResizeAnchor(AnchorViewCenter);
	setAlignment(Qt::AlignTop | Qt::AlignLeft);

	setMode(SELECT);

	drawLayout = 0;

	ghostArc  = 0;
	ghostEdge = 0;
	ghostVertex = new VertexDraw();

	setGraph(graph);

	centerOn(scene->itemsBoundingRect().center());

	connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(updateRect()));
	updateRect();
}

GraphDraw::~GraphDraw()
{
	if(drawLayout)
	{
		delete drawLayout;
	}
}

GraphDraw::InteractionMode GraphDraw::getMode()
{
	return mode;
}

void GraphDraw::setMode(InteractionMode newMode)
{
	scene->clearSelection();

	mode = newMode;

	if(mode == SELECT)
	{
		setCursor(Qt::ArrowCursor);
		setInteractive(true);
		setMouseTracking(false);
		setDragMode(RubberBandDrag);
	}
	else if(mode == PAN)
	{
		setInteractive(false);
		setMouseTracking(false);
		setDragMode(ScrollHandDrag);
	}
	else if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		setCursor(Qt::CrossCursor);
		setInteractive(false);
		setMouseTracking(true);
		setDragMode(NoDrag);
	}
}

DrawTables const* GraphDraw::getTables()
{
	return &tables;
}

Graph* GraphDraw::getGraph()
{
	return graph;
}



void GraphDraw::setGraph(Graph *newGraph)
{
	disconnect(graph, 0, this, 0);

	graph = newGraph;

	tables.A.clear();
	tables.E.clear();
	tables.V.clear();

	scene->clear();


	foreach(Vertex* vertex, graph->getVertexSet())
	{
		addVertex(vertex);
	}

	foreach(Edge* edge, graph->getEdgeSet())
	{
		addEdge(edge);
	}

	foreach(Arc* arc, graph->getArcSet())
	{
		addArc(arc);
	}

	connect(graph, SIGNAL(arcAdded(Arc*)),   this, SLOT(addArc(Arc*)));
	connect(graph, SIGNAL(arcFlipped(Arc*)), this, SLOT(flipArc(Arc*)));
	connect(graph, SIGNAL(arcDeleting(Arc*)), this, SLOT(deleteArc(Arc*)));

	connect(graph, SIGNAL(edgeAdded(Edge*)),   this, SLOT(addEdge(Edge*)));
	connect(graph, SIGNAL(edgeDeleting(Edge*)), this, SLOT(deleteEdge(Edge*)));

	connect(graph, SIGNAL(vertexAdded(Vertex*)),   this, SLOT(addVertex(Vertex*)));
	connect(graph, SIGNAL(vertexDeleting(Vertex*)), this, SLOT(deleteVertex(Vertex*)));

	connect(graph, SIGNAL(graphCleared()), this, SLOT(clearAll()));
}

QSize GraphDraw::sizeHint() const
{
	return QSize(300, 300);
}

void GraphDraw::contextMenuEvent(QContextMenuEvent *event)
{
	foreach(QGraphicsItem* item, items(event->pos()))
	{
		if(!item->isSelected())
		{
			scene->clearSelection();
			break;
		}
	}

	foreach(QGraphicsItem* item, items(event->pos()))
	{
		item->setSelected(true);
	}


	/* TODO: Make this menu enable and disable the different options based on the
			 content of the current selection */

	QMenu *contextMenu = new QMenu();

	contextMenu->addAction("Flip Arcs", this, SLOT(flipSelectedArcs()));
	contextMenu->addSeparator();
	contextMenu->addAction("Delete All", this, SLOT(deleteSelection()));
	contextMenu->exec(event->globalPos());
	delete contextMenu;
}

void GraphDraw::enterEvent(QEvent *event)
{
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		//ghostVertex = new VertexDraw();
		scene->addItem(ghostVertex);
		ghostVertex->setSelected(true);

		ghostVertex->setPos(mapToScene(mapFromGlobal(QCursor::pos())));

		if(mode == ADD_EDGE)
		{
			scene->addItem(ghostEdge);
		}
		else if(mode == ADD_ARC)
		{
			scene->addItem(ghostArc);
		}
	}

	QGraphicsView::enterEvent(event);
}

void GraphDraw::leaveEvent(QEvent *event)
{
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		scene->removeItem(ghostVertex);
		//delete ghostVertex;

		if(mode == ADD_EDGE)
		{
			scene->removeItem(ghostEdge);
		}
		else if(mode == ADD_ARC)
		{
			scene->removeItem(ghostArc);
		}
	}


	QGraphicsView::leaveEvent(event);
}

void GraphDraw::mouseMoveEvent(QMouseEvent *event)
{
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		ghostVertex->setVisible(true);
		VertexDraw* hover = getHoveredVertex();

		if(hover)
		{
			hover->setSelected(true);
			ghostVertex->setVisible(false);
		}
		else
		{
			scene->clearSelection();
			ghostVertex->setSelected(true);
		}

		ghostVertex->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
	}

	QGraphicsView::mouseMoveEvent(event);
}

void GraphDraw::mousePressEvent(QMouseEvent *event)
{
	if(mode == ADD_VERTEX)
	{
		graph->addVertex();
	}
	else if(mode == ADD_EDGE || mode == ADD_ARC)
	{
		if((mode == ADD_EDGE && ghostEdge) || (mode == ADD_ARC && ghostArc))
		{
			VertexDraw* vd2 = getHoveredVertex();

			if(vd2 == 0)
			{
				Vertex* v2 = graph->addVertex();
				vd2 = tables.V[v2];
				vd2->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
			}

			if(mode == ADD_EDGE)
			{
				graph->addEdge(ghostEdge->getV1()->getVertex(), vd2->getVertex());

				scene->removeItem(ghostEdge);
				delete ghostEdge;
				ghostEdge = 0;
			}
			else
			{
				graph->addArc(ghostArc->getTail()->getVertex(), vd2->getVertex());

				scene->removeItem(ghostArc);
				delete ghostArc;
				ghostArc = 0;
			}
		}
		else
		{
			VertexDraw* vd1 = getHoveredVertex();

			if(vd1 == 0)
			{
				Vertex* v1 = graph->addVertex();

				vd1 = tables.V[v1];
				vd1->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
			}

			if(mode == ADD_EDGE)
			{
				ghostEdge = new EdgeDraw(vd1, ghostVertex);
				scene->addItem(ghostEdge);
			}
			else
			{
				ghostArc = new ArcDraw(vd1, ghostVertex);
				scene->addItem(ghostArc);
			}
		}
	}

	QGraphicsView::mousePressEvent(event);
}

void GraphDraw::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);

	foreach(QGraphicsItem* item, items())
	{
		item->update();
	}
}

void GraphDraw::dragMoveEvent(QDragMoveEvent *event)
{
	QGraphicsView::dragMoveEvent(event);
}



/* TODO: rename thse to addArcDraw or arcAdded to better reflect their actual purpose */

void GraphDraw::addArc(Arc* arc)
{
	ArcDraw* ad = new ArcDraw(&tables, arc);
	tables.A.insert(arc, ad);

	scene->addItem(ad);
}

void GraphDraw::flipArc(Arc *arc)
{
	ArcDraw* ad = tables.A[arc];

	VertexDraw* temp = ad->head;
	ad->head = ad->tail;
	ad->tail = temp;

	ad->update();
}

/* TODO: maybe it would be a better idea to have the arc itself emit a flipped signal when
		 it's flipped so individual ArcDraws could listen to their own arc's signals and do
		 the flipping without having to involve the GraphDraw. Or maybe not...
*/

void GraphDraw::deleteArc(Arc* arc)
{
	ArcDraw* ad = tables.A.value(arc);
	tables.A.remove(arc);

	scene->removeItem(ad);
}

void GraphDraw::addEdge(Edge* edge)
{
	EdgeDraw* ed = new EdgeDraw(&tables, edge);
	tables.E.insert(edge, ed);

	scene->addItem(ed);
}

void GraphDraw::deleteEdge(Edge* edge)
{
	EdgeDraw* ed = tables.E.value(edge);
	tables.E.remove(edge);

	scene->removeItem(ed);
}


void GraphDraw::addVertex(Vertex* vertex)
{
	VertexDraw* vd = new VertexDraw(&tables, vertex);
	tables.V.insert(vertex, vd);

	scene->addItem(vd);

	if(mode == ADD_VERTEX)
	{
		vd->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
	}
}

void GraphDraw::deleteVertex(Vertex* vertex)
{
	VertexDraw* vd = tables.V.value(vertex);
	scene->removeItem(vd);
	tables.V.remove(vertex);

	// TODO: Figure our some way to force redraw of all the vertex draws (to get the new numeric labels)
}

void GraphDraw::clearAll()
{
	scene->clear();

	tables.V.clear();
	tables.E.clear();
	tables.A.clear();
}

void GraphDraw::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Delete)
	{
		deleteSelection();
	}
}

VertexDraw* GraphDraw::getHoveredVertex()
{
	foreach(VertexDraw* vd, tables.V)
	{
		if(vd->isUnderMouse())
		{
			return vd;
		}
	}

	return 0;
}

void GraphDraw::deleteSelection()
{
	foreach(ArcDraw* ad, tables.A)
	{
		if(ad->isSelected())
		{
			graph->removeArc(ad->getArc());
		}
	}

	foreach(EdgeDraw* ed, tables.E)
	{
		if(ed->isSelected())
		{
			graph->removeEdge(ed->getEdge());
		}
	}

	foreach(VertexDraw* vd, tables.V)
	{
		if(vd->isSelected())
		{
			graph->removeVertex(vd->getVertex());
		}
	}
}

void GraphDraw::flipSelectedArcs()
{
	foreach(ArcDraw* ad, tables.A)
	{
		if(ad->isSelected())
		{
			graph->flipArc(ad->getArc());
		}
	}
}

void GraphDraw::viewAll()
{
	fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GraphDraw::resetView()
{
	resetTransform();
}

void GraphDraw::updateRect()
{
	QRect visibleRect = visibleRegion().boundingRect();
	visibleRect.adjust(0, 0, -20, -20);

	if(horizontalScrollBar()->isVisible())
	{
		visibleRect.adjust(0, 0, 0, -1*horizontalScrollBar()->height()-2);
	}

	if(verticalScrollBar()->isVisible())
	{
		visibleRect.adjust(0, 0, -1*verticalScrollBar()->height()-2, 0);
	}

	scene->setSceneRect(scene->itemsBoundingRect().united(visibleRect));
}

void GraphDraw::doLayout(DrawLayout* newLayout, int minVertices)
{
	if(drawLayout)
	{
		delete drawLayout;
	}
	drawLayout = newLayout;

	drawLayout->layoutAuto(this, tables.V.values(), minVertices);
}
