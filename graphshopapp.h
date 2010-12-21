#ifndef GRAPHSHOPAPP_H
#define GRAPHSHOPAPP_H

#include <QApplication>
#include <QtScript>

#include "graphcore/_graphcore_c.h"

class GraphShopApp : public QApplication
{
    Q_OBJECT

public:
	explicit GraphShopApp(int& argc, char** argv);

	const QList<Graph*> graphList();
	QScriptEngine* scriptEngine();

signals:
	void graphAdded(Graph*);

public slots:
	Graph* addGraph(Graph*);	
	Graph* includeGraph(Graph*);

	Graph* addNewGraph(QString label="");

private:
	QList<Graph*> _graphList;
	QScriptEngine* _scriptEngine;

	void _setupScripting();
};


extern GraphShopApp* gsApp;

#endif // GRAPHSHOPAPP_H
