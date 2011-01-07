#include <QtGui>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "graphcore/representations/interval.h"

#include "intervaldraw.h"

#define STRETCH 20.0

IntervalDraw::IntervalDraw(Interval *sourceInterval)
{
	setLevel(0);
	_drag = DRAG_NONE;

	_source = sourceInterval;

	connect(_source, SIGNAL(moved(Interval*)), SLOT(updatePosition()));

	//setZValue(1);

	//setFlag(ItemIsMovable);
	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);
	//setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setAcceptHoverEvents(true);
}

void IntervalDraw::updatePosition()
{
	prepareGeometryChange();
}

int IntervalDraw::level()
{
	return _level;
}

void IntervalDraw::setLevel(int newlevel)
{
	bool changed = (_level != newlevel);

	_level = newlevel;
	setY(-20*_level);

	if(changed)
	{
		emit levelChanged(this);
	}
}

Interval* IntervalDraw::source()
{
	return _source;
}

void IntervalDraw::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if(event->scenePos().x() < _source->leftEdge() * STRETCH + 2)
	{
		setCursor(Qt::SizeHorCursor);
	}
	else if(event->scenePos().x() > _source->rightEdge() * STRETCH - 2)
	{
		setCursor(Qt::SizeHorCursor);
	}
	else
	{
		setCursor(Qt::SizeAllCursor);
	}
}

void IntervalDraw::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	unsetCursor();
}

void IntervalDraw::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);

	if(event->scenePos().x() < _source->leftEdge() * STRETCH + 2)
	{
		_drag = DRAG_LEFT;
	}
	else if(event->scenePos().x() > _source->rightEdge() * STRETCH - 2)
	{
		_drag = DRAG_RIGHT;
	}
	else
	{
		_drag = DRAG_CENTER;
	}

	_dragDelta = event->scenePos();
}


void IntervalDraw::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mouseMoveEvent(event);

	if(_drag != DRAG_NONE)
	{
		if(_drag == DRAG_LEFT && event->scenePos().x() > _source->rightEdge() * STRETCH)
		{
			_drag = DRAG_RIGHT;
		}
		else if(_drag == DRAG_RIGHT && event->scenePos().x() < _source->leftEdge() * STRETCH)
		{
			_drag = DRAG_LEFT;
		}


		if(_drag == DRAG_LEFT)
		{
			_source->moveLeftEdge((event->scenePos().x() - _dragDelta.x()) / STRETCH);
		}
		else if(_drag == DRAG_RIGHT)
		{
			_source->moveRightEdge((event->scenePos().x() - _dragDelta.x()) / STRETCH);
		}
		else if(_drag == DRAG_CENTER)
		{
			_source->move((event->scenePos().x() - _dragDelta.x()) / STRETCH);
		}

		_dragDelta = event->scenePos();
		update();
	}
}

void IntervalDraw::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	_drag = DRAG_NONE;
}




QRectF IntervalDraw::boundingRect() const
{
	return QRectF(_source->leftEdge()*STRETCH-2, -5, _source->width()*STRETCH+4, 10);
}

QPainterPath IntervalDraw::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void IntervalDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	double l = _source->leftEdge() *STRETCH;
	double r = _source->rightEdge()*STRETCH;

	int width = (isSelected() ? 3 : 1);


	//painter->setBrush(QBrush(background));
	painter->setPen(QPen(QBrush(Qt::black), width));

	painter->drawLine(l, -5, l, 5);
	painter->drawLine(l, 0, r, 0);
	painter->drawLine(r, -5, r, 5);

	//painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(index()));
	//painter->drawText(QPoint(0, 0), QString::number(x()));
}
