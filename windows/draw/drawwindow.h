#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include "graphdraw.h"
#include "windows/graphwindow.h"

class DrawWindow : public GraphWindow
{
	Q_OBJECT

public:
	DrawWindow(Graph* graph, GraphShopWindow* parent);

	GraphDraw* draw();

protected:
	void _build();
	//void keyPressEvent(QKeyEvent *);
	void resizeEvent(QResizeEvent *);

private:
	Graph* _graph;
	GraphDraw* _draw;

	QToolButton* _modeButton;
	QToolButton* _layoutButton;

private slots:
	void zoomIn();
	void zoomOut();
	void zoomAll();
	void zoomStd();
	void rotateCW();
	void rotateCCW();

	void setMode(QAction*);

	void layoutChange(QAction*);
	void layoutRandom();
	void layoutCircle();
	void layoutGravity();

	void updateTitle();
};

Q_DECLARE_METATYPE(DrawWindow*)

#endif // DRAWWINDOW_H
