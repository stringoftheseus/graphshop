#ifndef GRAPHFORMATGSP_H
#define GRAPHFORMATGSP_H

#include "graphcore/_graphcore_c.h"
#include "windows/_panels_c.h"

class GraphShopWindow;

class GraphFormatGSP
{
public:
	GraphFormatGSP(GraphShopWindow* window);

	void save(QTextStream& dest);
	bool load(QTextStream& source);

	void saveGraph(Graph* graph, QTextStream& dest);
	QString loadGraph(Graph* graph, QTextStream& source);

	void saveAdjMatrixWindow(AdjMatrixWindow* window, QTextStream& dest);
	QString loadAdjMatrixWindow(AdjMatrixWindow* window, QTextStream& source);

	void saveDrawWindow(DrawWindow* window, QTextStream& dest);
	QString loadDrawWindow(DrawWindow* window, QTextStream& source);

	void saveIntervalGraphWindow(IntervalGraphWindow* window, QTextStream& dest);
	QString loadIntervalGraphWindow(IntervalGraphWindow* window, QTextStream& source);

	void saveTournamentWindow(TournamentWindow* window, QTextStream& dest);
	QString loadTournamentWindow(TournamentWindow* window, QTextStream& source);

private:
	GraphShopWindow* _window;

	QHash<Graph*, QString> _graph_id;
	QHash<QString, Graph*> _id_graph;
};

#endif // GRAPHFORMATGSP_H
