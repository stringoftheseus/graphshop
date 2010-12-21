#ifndef intervalgraphdraw_H
#define intervalgraphdraw_H

#include <QGraphicsView>

class Interval;
class IntervalGraph;
class IntervalDraw;

class IntervalGraphDraw : public QGraphicsView
{
	Q_OBJECT

public:
	IntervalGraphDraw(IntervalGraph* source);
	~IntervalGraphDraw();

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
	void addIntervalDraw(Interval*);

	void keyPressEvent(QKeyEvent *event);

	IntervalDraw* getHoveredInterval();

	IntervalGraph* _source;

	QGraphicsScene *_scene;
	QGraphicsItem* _invalidNotice;

	QList<IntervalDraw*> _intervals;

private slots:
	void deleteSelection();

	void intervalAdded(Interval*);
	void intervalDeleted(Interval*);
	void intervalMoved(Interval*);

	void cleared();
	void validity(bool);
};

#endif
