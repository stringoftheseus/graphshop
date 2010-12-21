#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScriptEngine>

#include "codewindow.h"

CodeWindow::CodeWindow(Graph *graph, GraphShopWindow *parent): QDockWidget(parent), graph(graph)
{
	setObjectName("code_editor_window");
	setWindowTitle("Code Editor");


	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);

	scriptEditor = new QTextEdit();
	scriptEditor->setAcceptDrops(false);
	scriptEditor->setAcceptRichText(false);
	layout->addWidget(scriptEditor);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->setMargin(0);
	buttonLayout->addStretch(1);

	buttonRun = new QPushButton("Run");
	buttonLayout->addWidget(buttonRun);

	QWidget* buttonRow = new QWidget(this);
	buttonRow->setLayout(buttonLayout);
	layout->addWidget(buttonRow);

	QWidget* widget = new QWidget(this);
	widget->setLayout(layout);
	setWidget(widget);

	scriptEngine = gsApp->scriptEngine();

	connect(buttonRun, SIGNAL(clicked()), this, SLOT(runScript()));
//	connect(scriptEngine, SIGNAL(signalHandlerException(QScriptValue)), this, SLOT(scriptException(QScriptValue)));

	// TODO: Enable doskey-style Up button support for getting previous commands back
	// TODO: Enable more advanced code-completion support (somehow)
}


void CodeWindow::runScript()
{
	QScriptValue result = scriptEngine->evaluate(scriptEditor->toPlainText());
	if(result.isError())
	{
		QMessageBox::warning(this, "GraphShop Script Error", result.toString());
	}
}
