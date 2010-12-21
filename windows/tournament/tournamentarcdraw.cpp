#include <math.h>

#include <QPen>
#include <QBrush>
#include <QPainter>


#include "graphcore/graph.h"

#include "tournamentvertexdraw.h"
#include "tournamentarcdraw.h"

TournamentArcDraw::TournamentArcDraw(TournamentVertexDraw *tail, TournamentVertexDraw *head): _tail(tail), _head(head)
{
	setZValue(-1.0);

	setFlag(ItemIsFocusable);
	setFlag(ItemIsSelectable);

	// Really it isn't, but there's a right click focus bug unless you say that it is...
	//setFlag(ItemIsMovable);


	tail->setArc(head, this);
	head->setArc(tail, this);
}

void TournamentArcDraw::flip()
{
	TournamentVertexDraw* temp = _tail;
	_tail = _head;
	_head = temp;

	update();
}

TournamentVertexDraw* TournamentArcDraw::tail()
{
	return _tail;
}


TournamentVertexDraw* TournamentArcDraw::head()
{
	return _head;
}


QRectF TournamentArcDraw::boundingRect() const
{
	const QPointF p1 = mapFromItem(_tail, 0, 0);
	const QPointF p2 = mapFromItem(_head, 0, 0);

	const double x1 = p1.x();
	const double x2 = p2.x();

	int height = (x2 - x1) / 4;
	return QRectF(QPointF(x1, -height), QPointF(x2, height));
}

QPainterPath TournamentArcDraw::shape() const
{
	QPainterPath path;

	QPointF p1 = mapFromItem(_tail, 0, 0);
	QPointF p2 = mapFromItem(_head, 0, 0);

	double x1 = p1.x();
	double x2 = p2.x();

	if(x1 > x2)
	{
		qSwap(p1, p2);
		qSwap(x1, x2);
	}

	// TODO: Fix this, then make it selectable
	path.moveTo(p2);
	//path.lineTo(x1-2, 10);

	//path.addRect(0, 0, 20, 20);

	int height = (x2 - x1) / 4;

	int startAngle = 0;
	int spanAngle = 180;

	path.arcTo(QRectF(QPointF(x1, -height), QPointF(x2, height)), startAngle, spanAngle);
	path.arcTo(QRectF(QPointF(x1, -height), QPointF(x2, height)), spanAngle, -spanAngle);

	QPainterPathStroker stroker;
	stroker.setWidth(6);

	return stroker.createStroke(path);
}

void TournamentArcDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	if(!_tail->collidesWithItem(_head))
	{
		const QPointF p1 = mapFromItem(_tail, 0, 0);
		const QPointF p2 = mapFromItem(_head, 0, 0);

		const double x1 = p1.x();
		const double x2 = p2.x();

		if(_tail->order() < _head->order())
		{
			painter->setPen(QPen(QBrush(Qt::gray), isSelected() ? 3 : 1));
		}
		else
		{
			painter->setPen(QPen(QBrush(Qt::black), isSelected() ? 3 : 1));
		}

		int height = (x2 - x1) / 4;
		QRectF rectangle(QPointF(x1, -height), QPointF(x2, height));

		int startAngle = 0 * 16;
		int spanAngle = 180 * 16;

		painter->drawArc(rectangle, startAngle, spanAngle);

		//QList<QColor> colors;
		//colors << Qt::red << Qt::green << Qt::blue << Qt::yellow;

		//painter->setPen(QPen(QBrush(Qt::green), 1, Qt::DashLine));
		//painter->drawPolygon(shape().toFillPolygon());
	}
}
