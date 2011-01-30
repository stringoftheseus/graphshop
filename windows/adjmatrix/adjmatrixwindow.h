#ifndef adjmatrixwindow_h
#define adjmatrixwindow_h

#include <QtGui>

#include "windows/graphwindow.h"

class GraphShopWindow;

class AdjMatrixWindow : public GraphWindow
{
	Q_OBJECT

public:
	AdjMatrixWindow(Graph* graph, GraphShopWindow* parent);

protected:
	void _build();

	void setHeaderLabels();

	Graph* _graph;
	QTableWidget* table;

private slots:
	void edgeAdded(Edge*);
	void edgeDeleted(Vertex*, Vertex*);

	void vertexAdded(Vertex*);
	void vertexDeleted(int);

	void cellEdited(int, int);
	void updateCell(int, int);

	void clearAll();
	void updateTitle();
};

Q_DECLARE_METATYPE(AdjMatrixWindow*)

#endif // djmatrixwindow_h
