#include <QtGui>

#include "windows/draw/graphdraw.h"
#include "windows/draw/vertexdraw.h"

#include "randomlayout.h"

RandomLayout::RandomLayout()
{
}

void RandomLayout::layout()
{
	int margin = 2*vertexdraws[0]->radius();
	QRect viewRect = graphdraw->rect().adjusted(margin, margin, -margin, -margin);
	// TODO: adjust for scroll bars also...

	QPolygonF polygon = graphdraw->mapToScene(viewRect);
	QRectF rect = polygon.boundingRect();
	QPointF point;

	foreach(VertexDraw* vertexdraw, vertexdraws)
	{
		do
		{
			point = QPointF(qrand() / (RAND_MAX + 1.0) * (rect.right() + 1 - rect.left()) + rect.left(),
							qrand() / (RAND_MAX + 1.0) * (rect.bottom() + 1 - rect.top()) + rect.top());

		} while(!polygon.containsPoint(point, Qt::OddEvenFill));

		vertexdraw->setPos(point);
	}
}
