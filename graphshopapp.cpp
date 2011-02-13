#include "graphcore/_graphcore_h.h"
#include "graphcore/operations/_graphoperations_h.h"

#include "qScriptRegisterQObjectMetaType.h"

#include "graphshopapp.h"

GraphShopApp* gsApp = 0;

GraphShopApp::GraphShopApp(int& argc, char** argv) : QApplication(argc, argv)
{
	// Global app pointer, similar to qApp, so we don't have to cast it every time we use one of these functions
	gsApp = this;

	_setupScripting();
/*
	Graph* graph = addNewGraph("Graph 1");
	graph->addArc(0, 1);
	graph->addArc(0, 2);
	graph->addArc(2, 1);
	graph->addArc(3, 0);
	graph->addArc(2, 3);
	graph->addArc(1, 3);
	//graph->addEdge(4, 5);
	//graph->addEdge(6  4);
	//graph->addEdge(4, 8);

	graph = addNewGraph("Graph 2");
	graph->addEdge(0, 1);
	graph->addEdge(1, 2);
	graph->addEdge(2, 3);
	graph->addEdge(1, 3);
	graph->addVertex();

	graph = addNewGraph("Graph 3");
	graph->addVertex();

	graph = addNewGraph("Graph 4");
*/
}

void GraphShopApp::_setupScripting()
{
	_scriptEngine = new QScriptEngine(this);

	qScriptRegisterQObjectMetaType<Arc*>(_scriptEngine);
	qScriptRegisterQObjectMetaType<Edge*>(_scriptEngine);
	qScriptRegisterQObjectMetaType<Graph*>(_scriptEngine);
	qScriptRegisterQObjectMetaType<Vertex*>(_scriptEngine);

	qScriptRegisterMetaType<QList<Arc*> >(_scriptEngine, qScriptValueFromSequence<QList<Arc*> >, qScriptValueToSequence<QList<Arc*> >);
	qScriptRegisterMetaType<QList<Edge*> >(_scriptEngine, qScriptValueFromSequence<QList<Edge*> >, qScriptValueToSequence<QList<Edge*> >);
	qScriptRegisterMetaType<QList<Graph*> >(_scriptEngine, qScriptValueFromSequence<QList<Graph*> >, qScriptValueToSequence<QList<Graph*> >);
	qScriptRegisterMetaType<QList<Vertex*> >(_scriptEngine, qScriptValueFromSequence<QList<Vertex*> >, qScriptValueToSequence<QList<Vertex*> >);

	//qScriptRegisterQObjectMetaType<DrawWindow*>(_scriptEngine);
	//qScriptRegisterQObjectMetaType<AdjMatrixWindow*>(_scriptEngine);

	_scriptEngine->globalObject().setProperty("Graph", _scriptEngine->newFunction(Graph::sGraph));
	_scriptEngine->globalObject().setProperty("ComplementGraph", _scriptEngine->newFunction(ComplementGraph::sComplementGraph));
	_scriptEngine->globalObject().setProperty("ConverseGraph", _scriptEngine->newFunction(ConverseGraph::sConverseGraph));
	_scriptEngine->globalObject().setProperty("DominationGraph", _scriptEngine->newFunction(DominationGraph::sDominationGraph));
	_scriptEngine->globalObject().setProperty("UnderlyingGraph", _scriptEngine->newFunction(UnderlyingGraph::sUnderlyingGraph));


	_scriptEngine->globalObject().setProperty("ui", _scriptEngine->newQObject(this));
}


const QList<Graph*> GraphShopApp::graphList()
{
	return _graphList;
}

QScriptEngine* GraphShopApp::scriptEngine()
{
	return _scriptEngine;
}


Graph* GraphShopApp::addGraph(Graph* graph)
{
	_graphList.append(graph);

	emit graphAdded(graph);

	return graph;
}

Graph* GraphShopApp::addNewGraph(QString label)
{
	return addGraph(new Graph(label.isEmpty() ? "Graph "+QString::number(_graphList.size()+1) : label, this));
}

Graph* GraphShopApp::includeGraph(Graph *graph)
{
	if(_graphList.contains(graph) == false)
	{
		addGraph(graph);
	}

	return graph;
}
