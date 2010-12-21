#ifndef arcdraw_h
#define arcdraw_h

#include <QMenu>
#include <QObject>
#include <QGraphicsItem>

#include "graphcore/vertex.h"
#include "graphcore/arc.h"

#include "drawtables.h"

class ArcDraw: public QGraphicsItem
{
public:
	ArcDraw(VertexDraw *tail, VertexDraw *head);
	ArcDraw(DrawTables* drawtables, Arc* arc);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	Arc* getArc();
	VertexDraw* getTail();

	//int type() const;

private:
	VertexDraw *head, *tail;

	DrawTables* const tables;
	Arc* const arc;

	friend class GraphDraw;
};

#endif // arcdraw_h
