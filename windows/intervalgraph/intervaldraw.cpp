#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "graphcore/representations/interval.h"

#include "intervaldraw.h"

#define STRETCH 20

IntervalDraw::IntervalDraw(Interval *sourceInterval)
{
	setLevel(0);
	_drag = false;

	_source = sourceInterval;

	connect(source, SIGNAL(moved(Interval*)), SLOT(updatePosition()));

	//setZValue(1);

	//setFlag(ItemIsMovable);
	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);
	//setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
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
	setY(-30*_level);

	if(changed)
	{
		emit levelChanged(this);
	}
}

Interval* IntervalDraw::source()
{
	return _source;
}


void IntervalDraw::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	updateSequence();

	QGraphicsItem::mousePressEvent(event);

	_drag = true;
	_dragDelta = scenePos() - event->scenePos();
}

/*
void IntervalDraw::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mouseMoveEvent(event);

	if(_drag)
	{
		setPos(event->scenePos().x() + _dragDelta.x(), 0);

		if(next && mapFromItem(next, 0, 0).x() < 0)
		{
			swapOrders(this, next);
		}
		else if(prev && mapFromItem(prev, 0, 0).x() > 0)
		{
			swapOrders(this, prev);
		}
	}

	update();
}

void IntervalDraw::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	_drag = false;
	updatePosition();
}
*/



QRectF IntervalDraw::boundingRect() const
{
	return QRectF(_source->leftEdge()*STRETCH, -10, _source->width()*STRETCH, 20);
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

	painter->drawLine(l, -10, l, 10);
	painter->drawLine(l, 0, r, 0);
	painter->drawLine(r, -10, r, 10);

	//painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(index()));
	//painter->drawText(QPoint(0, 0), QString::number(x()));
}
