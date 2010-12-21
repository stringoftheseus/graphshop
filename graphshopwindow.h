#ifndef GRAPHSTUDIOWINDOW_H
#define GRAPHSTUDIOWINDOW_H

#include <QtGui>

#include "graphcore/_graphcore_c.h"

#include "graphshopapp.h"
#include "graphmenu.h"

class GraphPack;

class GraphShopWindow : public QMainWindow
{
	Q_OBJECT

public:
    GraphShopWindow();

public slots:
	void addGraph(Graph*);
	void setGraph(Graph*);

	void setFullScreen(bool fullscreen=true);

private:
	Graph* _graph;
	QHash<Graph*, GraphPack*> _graphpacks;

	QAction* viewSeparator;

	GraphMenu* graphMenu;
	QMenu* viewMenu;

private slots:
	void switchToGraph(Graph*);

signals:
	void graphSet(Graph*);
};

#endif // GRAPHSTUDIOWINDOW_H
