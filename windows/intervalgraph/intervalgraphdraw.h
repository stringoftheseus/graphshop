#ifndef intervalgraphdraw_H
#define intervalgraphdraw_H

#include <QGraphicsView>

//class Interval;

#include "graphcore/representations/interval.h"
#include "intervaldraw.h"

class IntervalGraph;
//class IntervalDraw;

class IntervalGraphDraw : public QGraphicsView
{
	Q_OBJECT

public:
	IntervalGraphDraw(IntervalGraph* source);

public slots:
	void viewAll();
	void addInterval();

protected:
	QSize sizeHint() const;
	void contextMenuEvent(QContextMenuEvent* event);

	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void mouseMoveEvent(QMouseEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);

private:
	void keyPressEvent(QKeyEvent *event);

	IntervalDraw* getHoveredInterval();
	void updateIntervalDrawLevel(IntervalDraw*);

	IntervalGraph* _source;

	QGraphicsScene* _scene;
	QGraphicsItem* _invalidNotice;

	QHash<Interval*, IntervalDraw*> _intervals;

private slots:
	void deleteSelection();

	void intervalAdded(Interval*);
	void intervalDeleted(int);

	void intersectionMade(Interval*, Interval*);
	void intersectionLost(Interval*, Interval*);

	void intervalDrawLevelChanged(IntervalDraw*);

	void cleared();
	void validity(bool);
};

#endif
