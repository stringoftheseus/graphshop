#ifndef intervaldraw_h
#define intervalvertexdraw_h

#include <QGraphicsItem>


class IntervalDraw: public QGraphicsItem
{
public:
	IntervalDraw(QList<IntervalDraw*>* vertices);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	int radius();

	int index();

	int order();
	void setOrder(int pos);
	static void swapOrders(IntervalDraw* one, IntervalDraw* two);

	TournamentArcDraw* getArc(IntervalDraw* vertex);
	void setArc(IntervalDraw* vertex, TournamentArcDraw* arc);
	void removeArc(IntervalDraw* vertex);

	void updateSequence();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	bool _drag;
	QPointF _dragDelta;


	int r;
	int _order;

	IntervalDraw* next;
	IntervalDraw* prev;

	QList<IntervalDraw*>* vertices;
	QHash<IntervalDraw*, TournamentArcDraw*> arcs;

	void updatePosition();
};

#endif // intervaldraw_h
