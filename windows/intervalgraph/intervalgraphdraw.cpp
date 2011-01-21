#include <QtGui>
#include <QKeyEvent>
#include <QContextMenuEvent>

#include "graphcore/representations/intervalgraph.h"
#include "graphcore/representations/interval.h"

#include "intervalgraphdraw.h"
#include "intervaldraw.h"

IntervalGraphDraw::IntervalGraphDraw(IntervalGraph *source): _source(source)
{
	//setMinimumSize(200, 200);

	_scene = new QGraphicsScene(this);
	_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	_scene->setBackgroundBrush(palette().background());

	//scene->setSceneRect(-100, -100, 100, 100);
	setScene(_scene);

	setViewportUpdateMode(FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorViewCenter);
	setResizeAnchor(AnchorViewCenter);

	//setMode(SELECT);

	setCursor(Qt::ArrowCursor);
	setInteractive(true);
	setMouseTracking(false);
	setDragMode(RubberBandDrag);

	_scene->clear();

	QFont invalidFont;
	invalidFont.setPixelSize(20);
	invalidFont.setBold(true);
	_invalidNotice = _scene->addText("Invalid Interval Graph", invalidFont);

	if(_source->valid())
	{
		_scene->removeItem(_invalidNotice);
	}


	for(int i=0; i<_source->intervalCount(); i++)
	{
		intervalAdded(_source->getInterval(i));
	}

	foreach(IntervalDraw* draw, _intervals)
	{
		 updateIntervalDrawLevel(draw);
	}

	connect(_source, SIGNAL(intervalAdded(Interval*)), SLOT(intervalAdded(Interval*)));
	connect(_source, SIGNAL(intervalDeleted(int)),     SLOT(intervalDeleted(int)));

	connect(_source, SIGNAL(intersectionMade(Interval*, Interval*)), SLOT(intersectionMade(Interval*, Interval*)));
	connect(_source, SIGNAL(intersectionLost(Interval*, Interval*)), SLOT(intersectionLost(Interval*, Interval*)));

	connect(_source, SIGNAL(cleared()), this, SLOT(cleared()));
	connect(_source, SIGNAL(validityChanged(bool)), this, SLOT(validity(bool)));
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
			_scene->clearSelection();
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
	_scene->addItem(id);
	_intervals.insert(interval, id);

	// New vertices are always added by IntervalGraph without any intersections yet
	connect(id, SIGNAL(levelChanged(IntervalDraw*)), SLOT(intervalDrawLevelChanged(IntervalDraw*)));
}

void IntervalGraphDraw::intervalDeleted(int index)
{
	foreach(IntervalDraw* draw, _intervals)
	{
		if(draw->source()->index() == index)
		{
			_scene->removeItem(draw);
			_intervals.remove(draw->source());
			break;
		}
	}

	// All the intersections should already have been taken care of by intersectionLost
}


void IntervalGraphDraw::intersectionMade(Interval* i1, Interval* i2)
{
	updateIntervalDrawLevel(_intervals[i1]);
}

void IntervalGraphDraw::intersectionLost(Interval* i1, Interval* i2)
{
	IntervalDraw* id1 = _intervals[i1];
	IntervalDraw* id2 = _intervals[i2];

	updateIntervalDrawLevel((id1->level() > id2->level()) ? id1 : id2);
}

void IntervalGraphDraw::intervalDrawLevelChanged(IntervalDraw* id)
{
	foreach(Interval* interval, _source->intersections(id->source()))
	{
		updateIntervalDrawLevel(_intervals[interval]);
	}
}

void IntervalGraphDraw::updateIntervalDrawLevel(IntervalDraw* intervalDraw)
{
	QList<Interval*> intersections = _source->intersections(intervalDraw->source());

	int level=-1;
	bool valid=false;

	while(!valid)
	{
		level++;
		valid = true;

		foreach(Interval* otherInterval, intersections)
		{
			if(_intervals[otherInterval]->level() == level)
			{
				valid = false;
				break;
			}
		}
	}

	intervalDraw->setLevel(level);
}



void IntervalGraphDraw::cleared()
{
	_scene->clear();

	_intervals.clear();
}

void IntervalGraphDraw::validity(bool valid)
{
	setEnabled(valid);

	if(valid)
	{
		_scene->removeItem(_invalidNotice);

		// TODO: Perform any necessary updates to stuff that might have happened during invalidness.
		// (in reality this probably means rebuilding the entire display)
	}
	else
	{
		_scene->addItem(_invalidNotice);
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
	fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void IntervalGraphDraw::addInterval()
{
	_source->addInterval();
}

