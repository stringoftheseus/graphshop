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
	void addArc(Arc*);
	void flipArc(Arc*);
	void deleteArc(Arc*);

	void addEdge(Edge*);
	void deleteEdge(Edge*);

	void addVertex(Vertex*);
	void deleteVertex(Vertex*);

	void clearAll();
	void updateTitle();
};

Q_DECLARE_METATYPE(AdjMatrixWindow*)

#endif // djmatrixwindow_h
