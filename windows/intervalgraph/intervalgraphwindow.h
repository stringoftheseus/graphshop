#ifndef INTERVALGRAPHWINDOW_H
#define INTERVALGRAPHWINDOW_H

#include "windows/graphwindow.h"

class IntervalGraph;
class IntervalGraphDraw;

class IntervalGraphWindow : public GraphWindow
{
	Q_OBJECT

public:
	IntervalGraphWindow(Graph* graph, GraphShopWindow* parent);
	IntervalGraphWindow(IntervalGraph* source, GraphShopWindow* parent);

protected:
	void _build();

private:
	IntervalGraph* _graph;
	IntervalGraphDraw* _draw;

private slots:
	void zoomIn();
	void zoomOut();
	void zoomAll();
	void addVertex();

	void updateTitle();
};

Q_DECLARE_METATYPE(IntervalGraphWindow*)

#endif
