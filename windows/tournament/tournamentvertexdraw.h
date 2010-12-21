#ifndef tournamentvertexdraw_h
#define tournamentvertexdraw_h

#include <QGraphicsItem>
#include "tournamentarcdraw.h"

class TournamentDraw;

class TournamentVertexDraw: public QGraphicsItem
{
public:
	TournamentVertexDraw(QList<TournamentVertexDraw*>* vertices);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	int radius();

	int index();

	int order();
	void setOrder(int pos);
	static void swapOrders(TournamentVertexDraw* one, TournamentVertexDraw* two);

	TournamentArcDraw* getArc(TournamentVertexDraw* vertex);
	void setArc(TournamentVertexDraw* vertex, TournamentArcDraw* arc);
	void removeArc(TournamentVertexDraw* vertex);

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

	TournamentVertexDraw* next;
	TournamentVertexDraw* prev;

	QList<TournamentVertexDraw*>* vertices;
	QHash<TournamentVertexDraw*, TournamentArcDraw*> arcs;

	void updatePosition();
};

#endif // tournamentvertexitem_h
