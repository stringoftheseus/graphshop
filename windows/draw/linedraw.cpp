#include <QtGui>

#include "linedraw.h"

#include "graphcore/_graphcore_h.h"
#include "vertexdraw.h"

LineDraw::LineDraw(VertexDraw* t, VertexDraw* h, bool directed): _tail(t), _head(h),
																 _directed(directed)
{
	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);

	// Really it isn't, but there's a right click focus bug unless you say that it is...
	setFlag(ItemIsMovable);
}

VertexDraw* LineDraw::tail()
{
	return _tail;
}

VertexDraw* LineDraw::head()
{
	return _head;
}

void LineDraw::flip()
{
	qSwap(_tail, _head);
}

QRectF LineDraw::boundingRect() const
{
	return shape().boundingRect();
}

QPainterPath LineDraw::shape() const
{
	QPainterPathStroker shape;
	shape.setWidth(4);
	return shape.createStroke(linePath());
}

void LineDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(QBrush(Qt::black), isSelected() ? 2 : 1));
	painter->drawPath(linePath());
}

QPainterPath LineDraw::linePath() const
{
	QPainterPath path;

	if(_tail == _head)
	{
		// a. Draw a loop

		// a.1. Set radius parameters
		const int r = _tail->radius()+1; // radius of VertexDraw next to which we're drawing
		const double rb = 5*r;           // radius of circle for Bezier control points
		const int ra = r+5;              // radius of circle for arrow head line ends

		// a.2 Set angle parameters
		const double a = index();        // base angle (unique for each loop on a given vertex)
		const double ab  = 0.3;          // separation angle for Bezier control points
		const double aa = 0.6;           // separation angle for arrow head lines

		// a.3. Get base point (center of base VertexDraw tranlated to this item's coordinates)
		const QPointF p = mapFromItem(_tail, 0, 0);
		const double x = p.x();
		const double y = p.y();

		// a.4. Calculate Bezier contorl points (where point 0 is the same as point 3)
		const QPointF bc0(x-r*cos(a),     y+r*sin(a));
		const QPointF bc1(x-rb*cos(a-ab), y+rb*sin(a-ab));
		const QPointF bc2(x-rb*cos(a+ab), y+rb*sin(a+ab));

		// a.5 Create the path for the main loop
		path.moveTo(bc0);
		path.cubicTo(bc1, bc2, bc0);

		// a.6 If this is a directed loop, add the arrow head lines
		if(_directed)
		{
			path.moveTo(bc0);
			path.lineTo(bc0 + QPointF(-ra*cos(a), ra*sin(a)));

			path.moveTo(bc0);
			path.lineTo(bc0 + QPointF(-ra*cos(a+aa), ra*sin(a+aa)));
		}
	}
	else if(!_tail->collidesWithItem(_head))
	{

	}

	return path;



/*



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

		for(int i=-2; i<=2; i++)
		{
			const double bcdist = pow(dist, 0.5)*i;
			const double a = 0.3*i;
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
		*/

}

