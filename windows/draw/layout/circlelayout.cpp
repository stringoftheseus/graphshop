#include <QtGui>
#include <QMultiMap>

#include "windows/draw/graphdraw.h"
#include "windows/draw/vertexdraw.h"

#include "circlelayout.h"

CircleLayout::CircleLayout()
{
}

void CircleLayout::layout()
{
	int total = vertexdraws.count();
	QPointF center = centerOf(vertexdraws);
	float radius = qMax(25.0, vertexdraws[0]->radius()*total/1.0);

	QMultiMap<double, VertexDraw*> sorteddraws;

	foreach(VertexDraw* draw, vertexdraws)
	{
		sorteddraws.insert(lineAngle(center, draw->pos()), draw);
	}


	int i=0;
	foreach(VertexDraw* draw, sorteddraws)
	{
		double t = 6.28/total*i++;
		draw->setPos(center.x()+radius*sin(t), center.y()-radius*cos(t));
	}
}
