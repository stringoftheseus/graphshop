#ifndef vertexdraw_h
#define vertexdraw_h

#include <QGraphicsItem>
#include "graphcore/vertex.h"
#include "graphcore/edge.h"

#include "drawtables.h"

class VertexDraw: public QGraphicsItem
{
public:
	VertexDraw(DrawTables* drawtables=0, Vertex* vertex=0);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	Vertex* getVertex();
	int radius();

	//int type() const;
private:
	QString label();

	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	DrawTables* const tables;
	Vertex* const vertex;
	int r;
};

#endif // vertexdraw_h
