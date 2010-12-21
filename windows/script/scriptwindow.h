#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H

#include <QtGui>
#include <QtScript>

#include "graphcore/graph.h"
#include "graphshopwindow.h"

class ScriptWindow : public QDockWidget
{
	Q_OBJECT

public:
	ScriptWindow(Graph* graph, GraphShopWindow* parent);

private:
	Graph* graph;
	QScriptEngine* scriptEngine;

	QTextEdit *scriptHistory;
	QLineEdit *scriptEditor;

private slots:
	void runScript();

};

#endif // SCRIPTWINDOW_H
