#ifndef intervaldraw_h
#define intervalvertexdraw_h

#include <QGraphicsItem>

class Interval;

class IntervalDraw: public QGraphicsItem, QObject
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
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected slots:
	void updatePosition();

private:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	Interval* _source;

	bool _drag;
	QPointF _dragDelta;

	int _level;
};

#endif // intervaldraw_h
