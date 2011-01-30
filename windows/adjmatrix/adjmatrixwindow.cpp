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


	table = new QTableWidget(this);
	table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

	foreach(Vertex* vertex, _graph->getVertexSet())
	{
		vertexAdded(vertex);
	}

	foreach(Edge* edge, _graph->getEdgeSet())
	{
		edgeAdded(edge);
	}

	widget()->layout()->addWidget(table);


	connect(_graph, SIGNAL(edgeAdded(Edge*)),   this, SLOT(edgeAdded(Edge*)));
	connect(_graph, SIGNAL(edgeDeleted(Vertex*,Vertex*)), this, SLOT(edgeDeleted(Vertex*,Vertex*)));

	connect(_graph, SIGNAL(vertexAdded(Vertex*)),   this, SLOT(vertexAdded(Vertex*)));
	connect(_graph, SIGNAL(vertexDeleted(int)), this, SLOT(vertexDeleted(int)));

	connect(_graph, SIGNAL(graphCleared()), this, SLOT(clearAll()));
	connect(_graph, SIGNAL(labelChanged(QString)), SLOT(updateTitle()));

	connect(table, SIGNAL(cellChanged(int,int)), SLOT(cellEdited(int,int)));
}


void AdjMatrixWindow::vertexAdded(Vertex *vertex)
{
	int index = vertex->index();
	table->insertRow(index);
	table->insertColumn(index);

	table->setRowHeight(index, 20);
	table->setColumnWidth(index, 20);

	for(int r=0; r<table->rowCount(); r++)
	{
		QTableWidgetItem *item = new QTableWidgetItem("0");
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

		table->setItem(r, index, item);
		table->setItem(index, r, item->clone());
	}

	setHeaderLabels();
}

void AdjMatrixWindow::vertexDeleted(int index)
{
	table->removeRow(index);
	table->removeColumn(index);

	setHeaderLabels();
}

void AdjMatrixWindow::edgeAdded(Edge *edge)
{
	updateCell(edge->vertex1()->index(), edge->vertex2()->index());
}

void AdjMatrixWindow::edgeDeleted(Vertex *v1, Vertex *v2)
{
	updateCell(v1->index(), v2->index());
}

void AdjMatrixWindow::cellEdited(int row, int col)
{
	_graph->setEdgeMultiplicity(row, col, table->item(row, col)->text().toInt());
}

void AdjMatrixWindow::updateCell(int row, int col)
{
	table->item(row, col)->setText(QString::number(_graph->edgeMultiplicity(row, col)));
	table->item(col, row)->setText(QString::number(_graph->edgeMultiplicity(row, col)));
}

void AdjMatrixWindow::clearAll()
{
	table->clear();
}

void AdjMatrixWindow::updateTitle()
{
	setTitle("AdjMatrix: " + _graph->label());
}

void AdjMatrixWindow::setHeaderLabels()
{
	QStringList labels;
	for(int i=0; i<table->rowCount(); i++)
	{
		labels.append(QString::number(i));
	}

	table->setVerticalHeaderLabels(labels);
	table->setHorizontalHeaderLabels(labels);
}
