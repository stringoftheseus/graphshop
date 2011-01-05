#include <QKeyEvent>
#include <QContextMenuEvent>

#include "graphcore/representations/intervalgraph.h"
#include "graphcore/representations/interval.h"

#include "intervalgraphdraw.h"
#include "intervaldraw.h"

IntervalGraphDraw::IntervalGraphDraw(IntervalGraph *source): _source(source)
{
	//setMinimumSize(200, 200);

	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setBackgroundBrush(palette().background());

	//scene->setSceneRect(-100, -100, 100, 100);
	setScene(scene);

	setViewportUpdateMode(FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorViewCenter);
	setResizeAnchor(AnchorViewCenter);

	//setMode(SELECT);

	setCursor(Qt::ArrowCursor);
	setInteractive(true);
	setMouseTracking(false);
	setDragMode(RubberBandDrag);

	scene->clear();

	QFont invalidFont;
	invalidFont.setPixelSize(20);
	invalidFont.setBold(true);
	invalidNotice = scene->addText("Invalid Interval Graph", invalidFont);

	if(source->valid())
	{
		scene->removeItem(invalidNotice);
	}


	for(int i=0; i<source->intervalCount(); i++)
	{
		intervalAdded(source->getInterval(i));
	}

	connect(source, SIGNAL(intervalAdded(Interval*)), SLOT(intervalAdded(Interval*)));
	connect(source, SIGNAL(intervalDeleted(int)),     SLOT(intervalDeleted(int)));

	connect(source, SIGNAL(intersectionMade(Interval*, Interval*)), SLOT(intersectionMade(Interval*, Interval*)));
	connect(source, SIGNAL(intersectionLost(Interval*, Interval*)), SLOT(intersectionLost(Interval*, Interval*)));

	connect(source, SIGNAL(cleared()), this, SLOT(cleared()));
	connect(source, SIGNAL(validityChanged(bool)), this, SLOT(validity(bool)));
}




QSize IntervalGraphDraw::sizeHint() const
{
	return QSize(300, 300);
}

void IntervalGraphDraw::contextMenuEvent(QContextMenuEvent *event)
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

	contextMenu->addAction("Delete All", this, SLOT(deleteSelection()));
	contextMenu->exec(event->globalPos());
	delete contextMenu;
}

void IntervalGraphDraw::enterEvent(QEvent *event)
{/*
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
*/
	QGraphicsView::enterEvent(event);
}

void IntervalGraphDraw::leaveEvent(QEvent *event)
{/*
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
*/

	QGraphicsView::leaveEvent(event);
}

void IntervalGraphDraw::mouseMoveEvent(QMouseEvent *event)
{/*
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
*/
	QGraphicsView::mouseMoveEvent(event);
}

void IntervalGraphDraw::mousePressEvent(QMouseEvent *event)
{/*
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
*/
	QGraphicsView::mousePressEvent(event);
}

void IntervalGraphDraw::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
/*
	foreach(QGraphicsItem* item, items())
	{
		item->update();
	}*/
}

void IntervalGraphDraw::dragMoveEvent(QDragMoveEvent *event)
{
	QGraphicsView::dragMoveEvent(event);
}


void IntervalGraphDraw::intervalAdded(Interval* interval)
{
	IntervalDraw* id = new IntervalDraw(interval);
	scene->addItem(id);

	// New vertices are always added by IntervalGraph without any intersections yet
	connect(id, SIGNAL(levelChanged(IntervalDraw*)), SLOT(intervalDrawLevelChanged(IntervalDraw*)));
}

void IntervalGraphDraw::intervalDeleted(int)
{
	IntervalDraw* id = _intervals[index];
	scene->removeItem(id);

	_intervals.removeAll(id);

	// All the intersections should already have been taken care of by intersectionLost
}

void IntervalGraphDraw::intersectionMade(Interval* i1, Interval i2)
{
	updateIntervalDrawLevel(_intervals[i1->index()]);
}

void IntervalGraphDraw::intersectionLost(Interval* i1, Interval i2)
{
	IntervalDraw* id1 = _intervals[i1->index()];
	IntervalDraw* id2 = _intervals[i2->index()];

	updateIntervalDrawLevel((id1->level() > id2->level()) ? id1 : id2);
}

void IntervalGraphDraw::intervalDrawLevelChanged(IntervalDraw* id)
{
	foreach(Interval* interval, _source->intersetions(id->source()))
	{
		updateIntervalDrawLevel(_intervals[interval]);
	}
}

void IntervalGraphDraw::updateIntervalDrawLevel(IntervalDraw* intervalDraw)
{
	int level=0;
	QList<Interval*> intersections = _source->intersetions(intervalDraw->source());

	do
	{
		foreach(Interval* otherInterval, intersections)
		{
			if(_intervals[otherDraw]->level() == level)
			{
				continue;
			}
		}

		break;

	} while(++level);


	intervalDraw->setLevel(level);
}



void IntervalGraphDraw::cleared()
{
	scene->clear();

	_intervals.clear();
}

void IntervalGraphDraw::validity(bool valid)
{
	setEnabled(valid);

	if(valid)
	{
		scene->removeItem(invalidNotice);

		// TODO: Perform any necessary updates to stuff that might have happened during invalidness.
		// (in reality this probably means rebuilding the entire display)
	}
	else
	{
		scene->addItem(invalidNotice);
	}
}

void IntervalGraphDraw::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Delete)
	{
		deleteSelection();
	}
}

IntervalDraw* IntervalGraphDraw::getHoveredInterval()
{
	foreach(IntervalDraw* id, _intervals)
	{
		if(id->isUnderMouse())
		{
			return id;
		}
	}

	return 0;
}

void IntervalGraphDraw::deleteSelection()
{
	foreach(IntervalDraw* id, _intervals)
	{
		if(id->isSelected())
		{
			_source->deleteInterval(id->source());
		}
	}
}

void IntervalGraphDraw::viewAll()
{
	fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

