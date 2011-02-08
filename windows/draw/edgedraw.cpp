#include <math.h>

#include <QPen>
#include <QBrush>
#include <QPainter>

#include "graphcore/graph.h"

#include "vertexdraw.h"
#include "edgedraw.h"

EdgeDraw::EdgeDraw(VertexDraw *v1, VertexDraw *v2): v1(v1), v2(v2), tables(0), edge(0)
{

}

EdgeDraw::EdgeDraw(DrawTables *drawtables, Edge *edge): tables(drawtables), edge(edge)
{
	//setZValue(1.0);

	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);

	v1 = tables->V.value(edge->vertex1());
	v2 = tables->V.value(edge->vertex2());
}

Edge* EdgeDraw::getEdge()
{
	return edge;
}

VertexDraw* EdgeDraw::getV1()
{
	return v1;
}

QRectF EdgeDraw::boundingRect() const
{
	return QRectF(mapFromItem(v1, 0, 0), mapFromItem(v2, 0, 0)).normalized();
}

QPainterPath EdgeDraw::shape() const
{
	QPainterPath path;


	const int r1 = v1->radius()+1;
	const int r2 = v2->radius()+1;

	const QPointF p1 = mapFromItem(v1, 0, 0);
	const QPointF p2 = mapFromItem(v2, 0, 0);

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

void EdgeDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	if(!v1->collidesWithItem(v2))
	{
		const int r1 = v1->radius()+1;
		const int r2 = v2->radius()+1;
		const int ra = 8;

		const QPointF p1 = mapFromItem(v1, 0, 0);
		const QPointF p2 = mapFromItem(v2, 0, 0);

		const double x1 = p1.x();
		const double y1 = p1.y();
		const double x2 = p2.x();
		const double y2 = p2.y();

		const double dx = x2 - x1;
		const double dy = y2 - y1;

		const double dist   = sqrt(dx*dx + dy*dy);

		const double cost = dx / dist;
		const double sint = dy / dist;

		const int vert = y1 > y2 ? 1 : -1;
		const double t = acos(cost) * vert;

		for(int i=-2; i<=2; i+=2)
		{
			const double bcdist = pow(dist, 0.5)*i;
			const double a = 0.2*i;
			const double d = 0.4;

			const QPointF pc((x1+x2)/2,(y1+y2)/2);
			const QPointF bc1(pc.x()+(y1-y2)/dist*bcdist, pc.y()+(x2-x1)/dist*bcdist);

			//painter->setPen(QPen(QBrush(Qt::black), isSelected() ? 2 : 1));
			//painter->drawLine(x1+r1*sin(a), y1+r2*cos(a), x2-r2*sin(a), y2-r2*cos(a));

			//painter->setPen(QPen(QBrush(Qt::blue), 1));
			//painter->drawLine(pc, bc1);


			QPainterPath edgebezier;
			edgebezier.moveTo(             x1+r1*cos(t-a), y1-r2*sin(t-a));
			edgebezier.quadTo(bc1, QPointF(x2-r2*cos(t+a), y2+r2*sin(t+a)));

			painter->strokePath(edgebezier, QPen(QBrush(Qt::black), isSelected() ? 2 : 1));

			painter->setPen(QPen(QBrush(Qt::black), isSelected() ? 2 : 1));
			painter->drawLine(x2-r2*cos(t+a), y2+r2*sin(t+a), x2-r2*cos(t+a)-ra*cos(t+a+d), y2+r2*sin(t+a)+ra*sin(t+a+d));
			painter->drawLine(x2-r2*cos(t+a), y2+r2*sin(t+a), x2-r2*cos(t+a)-ra*cos(t+a-d), y2+r2*sin(t+a)+ra*sin(t+a-d));
		}
	}
}
