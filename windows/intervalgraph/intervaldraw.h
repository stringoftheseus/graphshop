#ifndef intervaldraw_h
#define intervalvertexdraw_h

#include <QGraphicsItem>

class Interval;

class IntervalDraw: public QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	IntervalDraw(Interval* sourceInterval);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	int level();
	void setLevel(int newlevel);

	Interval* source();

signals:
	void levelChanged(IntervalDraw*);

protected:
	enum {DRAG_NONE, DRAG_LEFT, DRAG_CENTER, DRAG_RIGHT} _drag;

	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected slots:
	void updatePosition();

private:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	Interval* _source;

	QPointF _dragDelta;

	int _level;
};

#endif // intervaldraw_h
