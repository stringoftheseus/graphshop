#ifndef GRAPHPACK_H
#define GRAPHPACK_H

#include <QObject>
#include <QMenu>

#include "graphcore/_graphcore_c.h"
#include "windows/_panels_c.h"

class GraphShopWindow;

class GraphPack : public QObject
{
    Q_OBJECT

public:
	GraphPack(Graph* graph, GraphShopWindow* parent);

	QMenu* panelMenu();
signals:

public slots:
	void build();

	void swapIn();
	void swapOut();

	void hidePanels();

private:
	bool built;
	QByteArray _panelState;

	Graph* _graph;
	GraphShopWindow* _parent;

	QMenu* _panelMenu;

	AdjMatrixWindow* _adjMatrixWindow;
	DrawWindow* _drawWindow;
	IntervalGraphWindow* _intervalGraphWindow;
	TournamentWindow* _tournamentWindow;

	GraphPack* _competitionGraph;
	GraphPack* _complementGraph;
	GraphPack* _converseGraph;
	GraphPack* _dominationGraph;
	GraphPack* _underlyingGraph;

private slots:
};

#endif // GRAPHPACK_H
