#ifndef codewindow_h
#define codewindow_h

#include <QtGui>
#include <QtScript>

#include "graphcore/graph.h"

class GraphShopWindow;

class CodeWindow : public QDockWidget
{
	Q_OBJECT

public:
	CodeWindow(Graph* graph, GraphShopWindow* parent);

private:
	Graph* graph;
	QScriptEngine* scriptEngine;
	QTextEdit *scriptEditor;

	QPushButton *buttonRun;

private slots:
	void runScript();

};

#endif // codewindow_h
