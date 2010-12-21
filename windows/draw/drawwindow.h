#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include "graphdraw.h"
#include "windows/graphwindow.h"

class DrawWindow : public GraphWindow
{
	Q_OBJECT

public:
	DrawWindow(Graph* graph, GraphShopWindow* parent);

protected:
	void _build();
	//void keyPressEvent(QKeyEvent *);

private:
	Graph* _graph;
	GraphDraw* draw;

private slots:
	void zoomIn();
	void zoomOut();
	void zoomAll();
	void rotateCW();
	void rotateCCW();

	void setMode(QAction*);

	void layoutRandom();
	void layoutCircle();
	void layoutGravity();

	void updateTitle();
};

Q_DECLARE_METATYPE(DrawWindow*)

#endif // DRAWWINDOW_H
