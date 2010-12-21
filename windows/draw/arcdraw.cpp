#include <math.h>

#include <QPen>
#include <QBrush>
#include <QPainter>


#include "graphcore/graph.h"

#include "vertexdraw.h"
#include "arcdraw.h"

/* TODO: We really need a way to combine EdgeDraw and ArcDraw into a single class, or at least
		 outsource the vast majority of the code (which is almost identical) to a single place
		 somewhere else.
*/

ArcDraw::ArcDraw(VertexDraw *tail, VertexDraw *head): head(head), tail(tail), tables(0), arc(0)
{

}

ArcDraw::ArcDraw(DrawTables *drawtables, Arc *arc): tables(drawtables), arc(arc)
{
	//setZValue(-1);

	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);

	// Really it isn't, but there's a right click focus bug unless you say that it is...
	setFlag(ItemIsMovable);

	head = tables->V.value(arc->head());
	tail = tables->V.value(arc->tail());
}

Arc* ArcDraw::getArc()
{
	return arc;
}

VertexDraw* ArcDraw::getTail()
{
	return tail;
}


QRectF ArcDraw::boundingRect() const
{
	return QRectF(mapFromItem(tail, 0, 0), mapFromItem(head, 0, 0)).normalized();
}

QPainterPath ArcDraw::shape() const
{
	QPainterPath path;
	//path.addRect(QRectF(mapFromItem(tail, 0, 0), mapFromItem(head, 0, 0)).normalized());

	const int r1 = tail->radius();
	const int r2 = head->radius();

	const QPointF p1 = mapFromItem(tail, 0, 0);
	const QPointF p2 = mapFromItem(head, 0, 0);

	const double x1 = p1.x();
	const double y1 = p1.y();
	const double x2 = p2.x();
	const double y2 = p2.y();

	const double dx = x2 - x1;
	const double dy = y2 - y1;
	const double dist = sqrt(dx*dx + dy*dy);

	const double cost = dx / dist;
	const double sint = dy / dist;

//	const int vert = y1 > y2 ? 1 : -1;
//	const double t = acos(cost) * vert;
//	const double a = 0.5;

	double w = 3;

	QPointF e1 = QPointF(x1+r1*cost, y1+r2*sint);
	QPointF e2 = QPointF(x2-r2*cost, y2-r2*sint);

	QPolygonF zone;

	zone << QPointF(e1.x() + w*sint, e1.y() - w*cost);
	zone << QPointF(e1.x() - w*sint, e1.y() + w*cost);
	zone << QPointF(e2.x() - w*sint, e2.y() + w*cost);
	zone << QPointF(e2.x() + w*sint, e2.y() - w*cost);
	zone << QPointF(e1.x() + w*sint, e1.y() - w*cost);

	path.addPolygon(zone);

	return path;
}

void ArcDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	if(!tail->collidesWithItem(head))
	{
		const int r1 = tail->radius();
		const int r2 = head->radius();
		const int ra = 10;

		const QPointF p1 = mapFromItem(tail, 0, 0);
		const QPointF p2 = mapFromItem(head, 0, 0);

		const double x1 = p1.x();
		const double y1 = p1.y();
		const double x2 = p2.x();
		const double y2 = p2.y();

		const double dx = x2 - x1;
		const double dy = y2 - y1;
		const double dist = sqrt(dx*dx + dy*dy);

		const double cost = dx / dist;
		const double sint = dy / dist;

		const int vert = y1 > y2 ? 1 : -1;
		const double t = acos(cost) * vert;
		const double a = 0.5;

		painter->setPen(QPen(QBrush(Qt::black), isSelected() ? 2 : 1));

		painter->drawLine(x1+r1*cost, y1+r2*sint, x2-r2*cost, y2-r2*sint);
		painter->drawLine(x2-r2*cost, y2-r2*sint, x2-r2*cost-ra*cos(a-t),  y2-r2*sint-ra*sin(a-t));
		painter->drawLine(x2-r2*cost, y2-r2*sint, x2-r2*cost-ra*cos(-a-t), y2-r2*sint-ra*sin(-a-t));

		//painter->setPen(QPen(QBrush(Qt::blue), 1));
		//painter->drawPath(shape());
	}
}
/*
int ArcDraw::type() const
{
	return UserType + 1;
}
*/
