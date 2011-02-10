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
		// a. Draw a loop from a vertex to itself

		// a.1. Set radius parameters
		const double r  = _tail->radius()+1; // radius of VertexDraw next to which we're drawing
		const double rb = 5*r;               // radius of circle for Bezier control points
		const double ra = r+5;               // radius of circle for arrow head line ends

		// a.2 Set angle parameters
		const double a = index();       // base angle (unique for each loop on a given vertex)
		const double b = 0.3;           // separation angle for Bezier control points
		const double s = 0.3;           // separation angle for arrow head lines

		// a.3. Get base point (center of base VertexDraw tranlated to this item's coordinates)
		const QPointF p = mapFromItem(_tail, 0, 0);
		const double x = p.x();
		const double y = p.y();

		// a.4. Calculate Bezier contorl points (where point 0 is the same as point 3)
		const QPointF bc0(x-r*cos(a),     y+r*sin(a));
		const QPointF bc1(x-rb*cos(a-b), y+rb*sin(a-b));
		const QPointF bc2(x-rb*cos(a+b), y+rb*sin(a+b));

		// a.5 Create the path for the main loop
		path.moveTo(bc0);
		path.cubicTo(bc1, bc2, bc0);

		// a.6 If this is a directed loop, add the arrow head lines
		if(_directed)
		{
			path.moveTo(bc0);
			path.lineTo(bc0 + QPointF(-ra*cos(a), ra*sin(a)));

			path.moveTo(bc0);
			path.lineTo(bc0 + QPointF(-ra*cos(a+2*s), ra*sin(a+2*s)));
		}
	}
	else if(!_tail->collidesWithItem(_head))
	{
		// b. Draw an edge or arc from one vertex to another

		// b.1. Calculate the draw orbit of this line
		const int i = index();
		const int q = ((i+1)/2)*pow(-1, i)*((_head > _tail) ? 1 : -1);

		// b.2. Get the radius and position of vertex1 (the tail)
		const int r1 = _tail->radius()+1;
		const QPointF p1 = mapFromItem(_tail, 0, 0);
		const double x1 = p1.x();
		const double y1 = p1.y();

		// b.3. Get the radius and position of vertex2 (the head)
		const int r2 = _head->radius()+1;
		const QPointF p2 = mapFromItem(_head, 0, 0);
		const double x2 = p2.x();
		const double y2 = p2.y();

		// b.4. Set angles, withds, and other configurable properties
		const double ra = r2+4;  // radius of circle for arrow head line ends
		const double dp = 0.5;   // distance of Bezier control point (as a power of the line length)
		const double a = 0.2*q;  // estimated relative ending angle of Bezier curve based on dp
		const double s = 0.3;    // separation angle for arrow head lines

		// b.5. Calculate line length and center point
		const double l = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
		const QPointF c((x1+x2)/2,(y1+y2)/2);

		// b.6. Calculate exact line angle
		const double t = acos((x2-x1)/l) * (y1 > y2 ? 1 : -1);

		// b.7. Calculate Bezier control points
		const double bcd = pow(l, dp)*q;                            // distanct from line to control point 1
		const QPointF bc0(x1+r1*cos(t-a), y1-r2*sin(t-a));          // line starting point
		const QPointF bc1(c+QPointF((y1-y2)/l*bcd, (x2-x1)/l*bcd)); // control point (off of line)
		const QPointF bc2(x2-r2*cos(t+a), y2+r2*sin(t+a));          // line ending point

		// b.8. Create the path for the line curve
		path.moveTo(bc0);
		path.quadTo(bc1, bc2);

		// b.9 If this is a directed loop, add the arrow head lines
		if(_directed)
		{
			path.moveTo(bc2);
			path.lineTo(bc2 + QPointF(-ra*cos(t+a+s), ra*sin(t+a+s)));

			path.moveTo(bc2);
			path.lineTo(bc2 + QPointF(-ra*cos(t+a-s), ra*sin(t+a-s)));
		}
	}

	return path;



/*



	if(!v1->collidesWithItem(v2))
	{



		*/

}

