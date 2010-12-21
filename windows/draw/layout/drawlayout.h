#ifndef DRAWLAYOUT_H
#define DRAWLAYOUT_H

#include <QtCore>

class GraphDraw;
class VertexDraw;

class DrawLayout: public QObject
{
	Q_OBJECT

public:
    DrawLayout();

	/* TODO: Really the best thing to do here would probably be to just pass
			 in the GraphDraw*, then have a function on that that could get
			 that vertexDraws from it, rather than passing both every time.
	*/

	DrawLayout(GraphDraw*, QList<VertexDraw*>);

	void setDraws(GraphDraw*, QList<VertexDraw*>);

	void layout(GraphDraw*, QList<VertexDraw*>);
	void layoutAuto(GraphDraw*, QList<VertexDraw*>, int minVertices = 1);
	void layoutSelected(GraphDraw*, QList<VertexDraw*>);

	static QList<VertexDraw*> selectedSubset(QList<VertexDraw*>);
	static QPointF centerOf(QList<VertexDraw*>);
	static QPointF centerOfSelected(QList<VertexDraw*>);
	static double lineAngle(QPointF p1, QPointF p2);

public slots:
	virtual void layout() = 0;

protected:
	GraphDraw* graphdraw;
	QList<VertexDraw*> vertexdraws;
};

#endif // DRAWLAYOUT_H
