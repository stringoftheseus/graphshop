#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "graphcore/graph.h"

#include "tournamentarcdraw.h"
#include "tournamentvertexdraw.h"

TournamentVertexDraw::TournamentVertexDraw(QList<TournamentVertexDraw *>* vertices): vertices(vertices)
{
	vertices->append(this);
	_order = index();
	_drag = false;

	//setZValue(1);
	r = 10;	

	//setFlag(ItemIsMovable);
	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);
	//setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);

	updatePosition();
}

int TournamentVertexDraw::radius()
{
	return r;
}

int TournamentVertexDraw::index()
{
	return vertices->indexOf(this);
}

int TournamentVertexDraw::order()
{
	return _order;
}

void TournamentVertexDraw::setOrder(int pos)
{
	_order = pos;

	updatePosition();
}

void TournamentVertexDraw::swapOrders(TournamentVertexDraw *one, TournamentVertexDraw *two)
{
	int oneorder = one->order();
	one->setOrder(two->order());
	two->setOrder(oneorder);

	one->updateSequence();
	two->updateSequence();
}

void TournamentVertexDraw::updateSequence()
{
	prev = 0;
	next = 0;

	foreach(TournamentVertexDraw* vertex, *vertices)
	{
		if(vertex->order() == order() - 1)
		{
			prev = vertex;
		}
		else if(vertex->order() == order() + 1)
		{
			next = vertex;
		}
	}
}

void TournamentVertexDraw::updatePosition()
{
	const int spacing = 60;

	setPos(order()*spacing, 0);
}

void TournamentVertexDraw::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	updateSequence();

	QGraphicsItem::mousePressEvent(event);

	_drag = true;
	_dragDelta = scenePos() - event->scenePos();
}

void TournamentVertexDraw::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mouseMoveEvent(event);

	if(_drag)
	{
		setPos(event->scenePos().x() + _dragDelta.x(), 0);
/*
		if(next && scenePos().x() > next->scenePos().x())
		{
			swapOrders(this, next);
		}
		else if(prev && scenePos().x() < prev->scenePos().x())
		{
			swapOrders(this, prev);
		}
*/
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

void TournamentVertexDraw::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	_drag = false;
	updatePosition();
}


TournamentArcDraw* TournamentVertexDraw::getArc(TournamentVertexDraw* vertex)
{
	return arcs[vertex];
}

void TournamentVertexDraw::setArc(TournamentVertexDraw* vertex, TournamentArcDraw* arc)
{
	arcs[vertex] = arc;
}

void TournamentVertexDraw::removeArc(TournamentVertexDraw* vertex)
{
	arcs.remove(vertex);

	if(vertex->order() < order())
	{
		_order--;
	}

	updatePosition();
}


/*
int TournamentVertexDraw::type() const
{
	return UserType + 2;
}*/


/*
QVariant TournamentVertexDraw::itemChange(GraphicsItemChange change, const QVariant &value)
{
	QVariant r = QGraphicsItem::itemChange(change, value);

	if (change == ItemPositionChange && scene())
	{
		foreach(Arc* arc, vertex->arcs())
		{
			tables->A.value(arc)->updatePosition();
		}
	}

	return r;
}*/

QRectF TournamentVertexDraw::boundingRect() const
{
	return QRectF(QPoint(-r, -r), QPoint(r, r));
}

QPainterPath TournamentVertexDraw::shape() const
{
	QPainterPath path;
	path.addEllipse(QPoint(), r, r);
	return path;
}

void TournamentVertexDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	/*if(vertex == 0)
	{
		painter->setBrush(QBrush(Qt::blue));
		painter->setPen(QPen(QBrush(Qt::black), 1));
		painter->drawEllipse(QPoint(), r, r);

		painter->setPen(QPen(QBrush(Qt::black), 1));
		painter->drawText(boundingRect(), Qt::AlignCenter, label());
	}
	else */

	QColor background = Qt::white;
	QColor foreground = Qt::black;

	if(isSelected())
	{
		foreground = Qt::white;
		background = Qt::black;
	}


	painter->setBrush(QBrush(background));
	painter->setPen(QPen(QBrush(Qt::black), 1));
	painter->drawEllipse(QPoint(), r, r);

	painter->setPen(QPen(QBrush(foreground), 1));
	painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(index()));

	//painter->drawText(QPoint(0, 0), QString::number(x()));
}
