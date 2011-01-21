#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QScriptEngine>

#include "graphshopwindow.h"
#include "scriptwindow.h"

ScriptWindow::ScriptWindow(Graph *graph, GraphShopWindow *parent): QDockWidget(parent), graph(graph)
{
	setWindowTitle("Script Editor");


	scriptHistory = new QTextEdit();
	scriptHistory->setReadOnly(true);

	scriptEditor  = new QLineEdit();



	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);

	layout->addWidget(scriptHistory);
	layout->addWidget(scriptEditor);

	QWidget* widget = new QWidget(this);
	widget->setLayout(layout);
	setWidget(widget);


	scriptEngine = gsApp->scriptEngine();


	connect(scriptEditor, SIGNAL(returnPressed()), this, SLOT(runScript()));
//	connect(scriptEngine, SIGNAL(signalHandlerException(QScriptValue)), this, SLOT(scriptException(QScriptValue)));
}

void ScriptWindow::runScript()
{
	scriptHistory->append(">> "+scriptEditor->text());

	QScriptValue result = scriptEngine->evaluate(scriptEditor->text());
	if(!result.isUndefined())
	{
		scriptHistory->append(result.toString());
		scriptHistory->append("");
	}

	scriptEditor->clear();
}
