#ifndef LINEDRAW_H
#define LINEDRAW_H

#include <QGraphicsItem>

#include "graphcore/_graphcore_c.h"
#include "drawtables.h"

class LineDraw : public QGraphicsItem
{
public:
	LineDraw(VertexDraw* t, VertexDraw* h, bool directed);

	VertexDraw* tail();
	VertexDraw* head();
	void flip();

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
	virtual int index() const = 0;

	QPainterPath linePath() const;

	VertexDraw * _tail;
	VertexDraw * _head;

private:
	bool _directed;
};

#endif // LINEDRAW_H
