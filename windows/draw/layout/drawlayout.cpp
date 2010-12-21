#include "windows/draw/vertexdraw.h"

#include "drawlayout.h"

DrawLayout::DrawLayout()
{
}

DrawLayout::DrawLayout(GraphDraw *graphDraw, QList<VertexDraw*> vertexDraws): graphdraw(graphDraw),
																			  vertexdraws(vertexDraws)
{

}

void DrawLayout::setDraws(GraphDraw *gd, QList<VertexDraw*> vds)
{
	graphdraw = gd;
	vertexdraws = vds;
}

void DrawLayout::layout(GraphDraw *gd, QList<VertexDraw*> vds)
{
	setDraws(gd, vds);
	layout();
}

void DrawLayout::layoutAuto(GraphDraw *gd, QList<VertexDraw*> vds, int minVertices)
{
	QList<VertexDraw*> selecteddraws = selectedSubset(vds);

	if(selecteddraws.count() >= minVertices)
	{
		setDraws(gd, selecteddraws);
	}
	else
	{
		setDraws(gd, vds);
	}

	layout();
}

void DrawLayout::layoutSelected(GraphDraw *graphdraw, QList<VertexDraw*> vertexdraws)
{
	layout(graphdraw, selectedSubset(vertexdraws));
}


QList<VertexDraw*> DrawLayout::selectedSubset(QList<VertexDraw*> draws)
{
	QList<VertexDraw*> sels;

	foreach(VertexDraw* draw, draws)
	{
		if(draw->isSelected())
		{
			sels << draw;
		}
	}

	return sels;
}

QPointF DrawLayout::centerOf(QList<VertexDraw*> draws)
{
	QVector<QPointF> points;

	foreach(VertexDraw* draw, draws)
	{
		points << draw->scenePos();
	}

	return QPolygonF(points).boundingRect().center();
}

QPointF DrawLayout::centerOfSelected(QList<VertexDraw*> draws)
{
	return centerOf(selectedSubset(draws));
}


double DrawLayout::lineAngle(QPointF p1, QPointF p2)
{
		const double dx = p2.x() - p1.x();
		const double dy = p2.y() - p1.y();
		const double dist = sqrt(dx*dx + dy*dy);

		const double cost = dy / dist;

		const int vert = p1.x() > p2.x() ? 1 : -1;
		const double t = acos(cost) * vert;

		return t;
}
