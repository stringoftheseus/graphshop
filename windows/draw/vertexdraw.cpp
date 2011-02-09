#include <QPen>
#include <QBrush>
#include <QPainter>

#include "graphcore/graph.h"

#include "arcdraw.h"
#include "edgedraw.h"
#include "vertexdraw.h"

VertexDraw::VertexDraw(DrawTables *drawtables, Vertex *vertex): tables(drawtables), vertex(vertex)
{
	//setZValue(1);
	r = 10;

	setPos(qrand()%201, qrand()%201);

	setFlag(ItemIsMovable);
	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);

	//setFlag(ItemSendsGeometryChanges);
	//setCacheMode(DeviceCoordinateCache);
}

int VertexDraw::radius() const
{
	return r;
}

/*
int VertexDraw::type() const
{
	return UserType + 2;
}*/

Vertex* VertexDraw::getVertex()
{
	return vertex;
}

/*
QVariant VertexDraw::itemChange(GraphicsItemChange change, const QVariant &value)
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

QRectF VertexDraw::boundingRect() const
{
	return QRectF(QPoint(-r, -r), QPoint(r, r));
}

QPainterPath VertexDraw::shape() const
{
	QPainterPath path;
	path.addEllipse(QPoint(), r, r);
	return path;
}

void VertexDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
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
/*	else
	{

		foreach(Vertex* v, vertex->outNeighborhood())
		{
			if(tables->V[v]->isSelected())
			{
				background = Qt::red;
			}
		}

		foreach(Vertex* v, vertex->inNeighborhood())
		{
			if(tables->V[v]->isSelected())
			{
				background = Qt::green;
			}
		}
	}
*/

	painter->setBrush(QBrush(background));
	painter->setPen(QPen(QBrush(Qt::black), 1));

	painter->drawEllipse(QPoint(), r, r);

	painter->setPen(QPen(QBrush(foreground), 1));
	painter->drawText(boundingRect(), Qt::AlignCenter, label());
}

QString VertexDraw::label()
{
	if(vertex)
	{
		return QString::number(vertex->index());
	}
	else
	{
		return "";
	}
}
