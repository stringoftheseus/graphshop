#ifndef graphdraw_H
#define graphdraw_H

#include <QGraphicsView>
#include "graphcore/graph.h"

#include "windows/graphgraphicsview.h"
#include "drawtables.h"

class DrawLayout;

class GraphDraw : public GraphGraphicsView
{
	Q_OBJECT

public:
	GraphDraw(Graph* graph);
	~GraphDraw();

	enum InteractionMode {SELECT, PAN, ADD_VERTEX, ADD_EDGE, ADD_ARC};
	InteractionMode getMode();
	void setMode(InteractionMode);

	void setGraph(Graph*);
	Graph* getGraph();

	DrawTables const* getTables();

public slots:
	void addArc(Arc*);
	void flipArc(Arc*);
	void deleteArc(Arc*);

	void addEdge(Edge*);
	void deleteEdge(Edge*);

	void addVertex(Vertex*);
	void deleteVertex(Vertex*);

	void clearAll();

	void viewAll();
	void resetView();
	void updateRect();

	void doLayout(DrawLayout*, int minVertices = 1);

protected:
	QSize sizeHint() const;
	void contextMenuEvent(QContextMenuEvent * event);

	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mouseMoveEvent(QMouseEvent *event);
	void showEvent(QShowEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);

private:
	void keyPressEvent(QKeyEvent *event);

	VertexDraw* getHoveredVertex();

	InteractionMode mode;

	DrawTables tables;
	Graph* graph;
	QGraphicsScene *scene;
	DrawLayout* drawLayout;

	VertexDraw* ghostVertex;
	EdgeDraw*   ghostEdge;
	ArcDraw*   ghostArc;

private slots:
	void deleteSelection();
	void flipSelectedArcs();
};

#endif // graphdraw_H
