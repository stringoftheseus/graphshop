#ifndef GRAPHSTUDIOWINDOW_H
#define GRAPHSTUDIOWINDOW_H

#include <QtGui>

#include "graphcore/_graphcore_c.h"

#include "graphshopapp.h"
#include "graphmenu.h"
#include "windows/script/codewindow.h"
#include "windows/script/scriptwindow.h"

class GraphPack;

class GraphShopWindow : public QMainWindow
{
	Q_OBJECT

public:
    GraphShopWindow();

public slots:
	void addGraph(Graph*);
	void setGraph(Graph*);

	void loadGraph();
	void saveGraph(Graph*);

	void setFullScreen(bool fullscreen=true);

	CodeWindow* showCodePanel(bool show);
	ScriptWindow* showScriptPanel(bool show);

private:
	Graph* _graph;
	QHash<Graph*, GraphPack*> _graphpacks;

	ScriptWindow* _scriptPanel;
	CodeWindow* _codePanel;

	QMenu* saveGraphMenu;

	QAction* viewSeparator;
	QMenu* viewMenu;

	bool _wasMaximized;

private slots:
	void switchToGraph(Graph*);

signals:
	void graphSet(Graph*);
};

#endif // GRAPHSTUDIOWINDOW_H
