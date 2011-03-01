#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "graphcore/arc.h"
#include "graphcore/edge.h"
#include "graphcore/graph.h"
#include "graphcore/vertex.h"

#include "adjmatrixwindow.h"

AdjMatrixWindow::AdjMatrixWindow(Graph *graph, GraphShopWindow *parent): GraphWindow(parent), _graph(graph)
{
	updateTitle();
}

void AdjMatrixWindow::_build()
{
	_graph->build();


	typeBox = new QComboBox();

	typeBox->addItem(QIcon("img/add_edge.png"), "Edges", EDGES);
	typeBox->addItem(QIcon("img/add_arc.png"), "Arcs", ARCS);

	_toolbar->addWidget(typeBox);

	connect(typeBox, SIGNAL(currentIndexChanged(int)), SLOT(changeType()));


	arcTable = new QTableWidget(this);
	arcTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed);

	edgeTable = new QTableWidget(this);
	edgeTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::AnyKeyPressed);


	foreach(Vertex* vertex, _graph->getVertexSet())
	{
		vertexAdded(vertex);
	}

	foreach(Arc* arc, _graph->getArcSet())
	{
		arcAdded(arc);
	}

	foreach(Edge* edge, _graph->getEdgeSet())
	{
		edgeAdded(edge);
	}

	widget()->layout()->addWidget(arcTable);
	widget()->layout()->addWidget(edgeTable);


	connect(_graph, SIGNAL(arcAdded(Arc*)),   this, SLOT(arcAdded(Arc*)));
	connect(_graph, SIGNAL(arcFlipped(Arc*)),   this, SLOT(arcFlipped(Arc*)));
	connect(_graph, SIGNAL(arcDeleted(Vertex*,Vertex*)), this, SLOT(arcDeleted(Vertex*,Vertex*)));

	connect(_graph, SIGNAL(edgeAdded(Edge*)),   this, SLOT(edgeAdded(Edge*)));
	connect(_graph, SIGNAL(edgeDeleted(Vertex*,Vertex*)), this, SLOT(edgeDeleted(Vertex*,Vertex*)));

	connect(_graph, SIGNAL(vertexAdded(Vertex*)),   this, SLOT(vertexAdded(Vertex*)));
	connect(_graph, SIGNAL(vertexDeleted(int)), this, SLOT(vertexDeleted(int)));

	connect(_graph, SIGNAL(graphCleared()), this, SLOT(clearAll()));
	connect(_graph, SIGNAL(labelChanged(QString)), SLOT(updateTitle()));

	connect(arcTable, SIGNAL(cellChanged(int,int)), SLOT(arcCellEdited(int,int)));
	connect(edgeTable, SIGNAL(cellChanged(int,int)), SLOT(edgeCellEdited(int,int)));

	changeType();
}

void AdjMatrixWindow::changeType()
{
	if(typeBox->itemData(typeBox->currentIndex()) == EDGES)
	{
		arcTable->setVisible(false);
		edgeTable->setVisible(true);
	}
	else
	{
		arcTable->setVisible(true);
		edgeTable->setVisible(false);
	}
}


void AdjMatrixWindow::vertexAdded(Vertex *vertex)
{
	int index = vertex->index();

	arcTable->insertRow(index);
	arcTable->insertColumn(index);

	arcTable->setRowHeight(index, 20);
	arcTable->setColumnWidth(index, 20);

	edgeTable->insertRow(index);
	edgeTable->insertColumn(index);

	edgeTable->setRowHeight(index, 20);
	edgeTable->setColumnWidth(index, 20);


	QTableWidgetItem *item = new QTableWidgetItem("0");
	item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

	for(int r=0; r<arcTable->rowCount(); r++)
	{
		arcTable->setItem(r, index, item->clone());
		arcTable->setItem(index, r, item->clone());

		edgeTable->setItem(r, index, item->clone());
		edgeTable->setItem(index, r, item->clone());
	}

	setHeaderLabels();
}

void AdjMatrixWindow::vertexDeleted(int index)
{
	arcTable->removeRow(index);
	arcTable->removeColumn(index);

	edgeTable->removeRow(index);
	edgeTable->removeColumn(index);

	setHeaderLabels();
}


void AdjMatrixWindow::arcAdded(Arc *arc)
{
	updateArcCell(arc->tail()->index(), arc->head()->index());
}

void AdjMatrixWindow::arcDeleted(Vertex *t, Vertex *h)
{
	updateArcCell(t->index(), h->index());
}

void AdjMatrixWindow::arcFlipped(Arc *arc)
{
	updateArcCell(arc->head()->index(), arc->tail()->index());
	updateArcCell(arc->tail()->index(), arc->head()->index());
}


void AdjMatrixWindow::edgeAdded(Edge *edge)
{
	updateEdgeCell(edge->vertex1()->index(), edge->vertex2()->index());
}

void AdjMatrixWindow::edgeDeleted(Vertex *v1, Vertex *v2)
{
	updateEdgeCell(v1->index(), v2->index());
}

void AdjMatrixWindow::arcCellEdited(int row, int col)
{
	_graph->setArcMultiplicity(row, col, arcTable->item(row, col)->text().toInt());
}

void AdjMatrixWindow::updateArcCell(int row, int col)
{
	arcTable->item(row, col)->setText(QString::number(_graph->arcMultiplicity(row, col)));
}

void AdjMatrixWindow::edgeCellEdited(int row, int col)
{
	_graph->setEdgeMultiplicity(row, col, edgeTable->item(row, col)->text().toInt());
}

void AdjMatrixWindow::updateEdgeCell(int row, int col)
{
	edgeTable->item(row, col)->setText(QString::number(_graph->edgeMultiplicity(row, col)));
	edgeTable->item(col, row)->setText(QString::number(_graph->edgeMultiplicity(row, col)));
}

void AdjMatrixWindow::clearAll()
{
	arcTable->clear();
	edgeTable->clear();
}

void AdjMatrixWindow::updateTitle()
{
	setTitle("AdjMatrix: " + _graph->label());
}

void AdjMatrixWindow::setHeaderLabels()
{
	QStringList labels;
	for(int i=0; i<arcTable->rowCount(); i++)
	{
		labels.append(QString::number(i));
	}

	arcTable->setVerticalHeaderLabels(labels);
	arcTable->setHorizontalHeaderLabels(labels);

	edgeTable->setVerticalHeaderLabels(labels);
	edgeTable->setHorizontalHeaderLabels(labels);
}
