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
	enum {EDGES, ARCS} type;

	void _build();

	void setHeaderLabels();

	Graph* _graph;

	QComboBox* typeBox;

	QTableWidget* arcTable;
	QTableWidget* edgeTable;


private slots:
	void arcAdded(Arc*);
	void arcDeleted(Vertex*, Vertex*);
	void arcFlipped(Arc*);

	void edgeAdded(Edge*);
	void edgeDeleted(Vertex*, Vertex*);

	void vertexAdded(Vertex*);
	void vertexDeleted(int);

	void arcCellEdited(int, int);
	void updateArcCell(int, int);

	void edgeCellEdited(int, int);
	void updateEdgeCell(int, int);

	void clearAll();
	void updateTitle();

	void changeType();
};

Q_DECLARE_METATYPE(AdjMatrixWindow*)

#endif // djmatrixwindow_h
