#ifndef INTERVAL_H
#define INTERVAL_H

#include <QObject>

class Vertex;

class Interval : public QObject
{
    Q_OBJECT

public:
	explicit Interval(double left, double right, Vertex* source);

	Vertex* sourceVertex();
	int index();

	double width();
	double center();
	double leftEdge();
	double rightEdge();

	bool intersects(Interval* that);

	void centerOn(double point);

	void moveLeftEdge(double delta);
	void moveRightEdge(double delta);
	void move(double delta);

	void setLeftEdge(double left);
	void setRightEdge(double right);
	void setEdges(double left, double right);

signals:
	void moved(Interval*);

protected:
	double _ledge, _redge;
	Vertex* _source;

	void normalize();

};

#endif // INTERVAL_H
