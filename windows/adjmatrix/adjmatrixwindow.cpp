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

	foreach(Vertex* vertex, _graph->getVertexSet())
	{
		addVertex(vertex);
	}

	foreach(Arc* arc, _graph->getArcSet())
	{
		addArc(arc);
	}

	foreach(Edge* edge, _graph->getEdgeSet())
	{
		addEdge(edge);
	}

	widget()->layout()->addWidget(table);


	connect(_graph, SIGNAL(arcAdded(Arc*)),   this, SLOT(addArc(Arc*)));
	connect(_graph, SIGNAL(arcFlipped(Arc*)), this, SLOT(flipArc(Arc*)));
	connect(_graph, SIGNAL(arcDeleting(Arc*)), this, SLOT(deleteArc(Arc*)));

	connect(_graph, SIGNAL(edgeAdded(Edge*)),   this, SLOT(addEdge(Edge*)));
	connect(_graph, SIGNAL(edgeDeleting(Edge*)), this, SLOT(deleteEdge(Edge*)));

	connect(_graph, SIGNAL(vertexAdded(Vertex*)),   this, SLOT(addVertex(Vertex*)));
	connect(_graph, SIGNAL(vertexDeleting(Vertex*)), this, SLOT(deleteVertex(Vertex*)));

	connect(_graph, SIGNAL(graphCleared()), this, SLOT(clearAll()));
	connect(_graph, SIGNAL(labelChanged(QString)), SLOT(updateTitle()));
}


void AdjMatrixWindow::addArc(Arc *arc)
{
	int row = arc->tail()->index();
	int col = arc->head()->index();

	int count1 = _graph->getArcs(arc->tail(), arc->head()).count();
	int count2 = _graph->getArcs(arc->head(), arc->tail()).count();

	table->item(row, col)->setText(QString::number(count1));
	table->item(col, row)->setText(QString::number(count2));
}

void AdjMatrixWindow::flipArc(Arc *arc)
{
	addArc(arc);
}

void AdjMatrixWindow::deleteArc(Arc *arc)
{
	int row = arc->tail()->index();
	int col = arc->head()->index();

	int count1 = _graph->getArcs(arc->tail(), arc->head()).count()-1;
	int count2 = _graph->getArcs(arc->head(), arc->tail()).count()-1;

	table->item(row, col)->setText(QString::number(count1));
	table->item(col, row)->setText(QString::number(count2));
}


void AdjMatrixWindow::addEdge(Edge *edge)
{
	int row = edge->vertex1()->index();
	int col = edge->vertex2()->index();

	int count = _graph->getEdges(edge->vertex1(), edge->vertex2()).count();

	table->item(row, col)->setText(QString::number(count));
	table->item(col, row)->setText(QString::number(count));
}

void AdjMatrixWindow::addVertex(Vertex *vertex)
{
	int index = vertex->index();
	table->insertRow(index);
	table->insertColumn(index);

	table->setRowHeight(index, 20);
	table->setColumnWidth(index, 20);

	for(int r=0; r<table->rowCount(); r++)
	{
		QTableWidgetItem *item = new QTableWidgetItem("0");
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		table->setItem(r, index, item);
		table->setItem(index, r, item->clone());
	}

	setHeaderLabels();
}



void AdjMatrixWindow::deleteEdge(Edge *edge)
{
	int row = edge->vertex1()->index();
	int col = edge->vertex2()->index();

	int count = _graph->getEdges(edge->vertex1(), edge->vertex2()).count()-1;

	table->item(row, col)->setText(QString::number(count));
	table->item(col, row)->setText(QString::number(count));
}

void AdjMatrixWindow::deleteVertex(Vertex *vertex)
{
	int index = vertex->index();

	table->removeRow(index);
	table->removeColumn(index);

	setHeaderLabels();
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
