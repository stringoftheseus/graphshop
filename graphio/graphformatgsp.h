#ifndef GRAPHFORMATGSP_H
#define GRAPHFORMATGSP_H

#include "graphcore/_graphcore_c.h"
#include "windows/_panels_c.h"

class GraphShopWindow;

class GraphFormatGSP
{
public:
	GraphFormatGSP(GraphShopWindow* window);

	bool load(QTextStream& source);
	void save(QTextStream& dest);

	QString loadGraph(QTextStream& source);
	void saveGraph(Graph* graph, QTextStream& dest);

	QString loadAdjMatrixWindow(QTextStream& source);
	void saveAdjMatrixWindow(AdjMatrixWindow* window, QTextStream& dest);

	QString loadDrawWindow(QTextStream& source);
	void saveDrawWindow(DrawWindow* window, QTextStream& dest);

	QString loadIntervalGraphWindow(QTextStream& source);
	void saveIntervalGraphWindow(IntervalGraphWindow* window, QTextStream& dest);

	QString loadTournamentWindow(QTextStream& source);
	void saveTournamentWindow(TournamentWindow* window, QTextStream& dest);


private:
	GraphShopWindow* _window;

	QHash<Graph*, QString> _graph_id;
	QHash<QString, Graph*> _id_graph;
};

#endif // GRAPHFORMATGSP_H
