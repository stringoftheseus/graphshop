#ifndef EDGEDRAW_H
#define EDGEDRAW_H

#include <QGraphicsItem>
#include "graphcore/vertex.h"
#include "graphcore/edge.h"

#include "drawtables.h"

class EdgeDraw: public QGraphicsItem
{
public:
	EdgeDraw(VertexDraw* v1, VertexDraw* v2);
	EdgeDraw(DrawTables* drawtables, Edge* edge);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	Edge* getEdge();
	VertexDraw* getV1();

private:
	VertexDraw *v1, *v2;

	DrawTables* const tables;
	Edge* const edge;
};

#endif // EDGEDRAW_H
