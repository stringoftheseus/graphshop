#ifndef tournamentarcdraw_h
#define tournamentarcdraw_h

#include <QMenu>
#include <QObject>
#include <QGraphicsItem>

#include "graphcore/vertex.h"
#include "graphcore/arc.h"

class TournamentVertexDraw;

class TournamentArcDraw: public QGraphicsItem
{
public:
	TournamentArcDraw(TournamentVertexDraw *tail, TournamentVertexDraw *head);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void flip();
	TournamentVertexDraw* head();
	TournamentVertexDraw* tail();

private:
	TournamentVertexDraw *_tail, *_head;
};

#endif // tournamentarcdraw_h
