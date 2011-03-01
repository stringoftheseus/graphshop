#include <QtGui>
#include "windows/draw/vertexdraw.h"
#include "rotatelayout.h"

RotateLayout::RotateLayout(double t): _t(t)
{
}

void RotateLayout::layout()
{
	QPointF center = centerOf(vertexdraws);

	foreach(VertexDraw* draw, vertexdraws)
	{
		QLineF line(center, draw->pos());
		line.setAngle(line.angle() - _t);
		draw->setPos(line.p2());
	}
}
