#include "graphcore/graph.h"
#include "graphio/graphformatgsg.h"

#include "graphpack.h"

#include "graphshopwindow.h"


GraphShopWindow::GraphShopWindow()
{
	setWindowTitle("GraphShop");
	setWindowState(Qt::WindowActive | Qt::WindowMaximized);

	_graph = 0;
	_scriptPanel = 0;
	_codePanel = 0;

	QMenuBar* menubar = menuBar();

	QMenu* fileMenu = menubar->addMenu("&File");
		QMenu* fileNewMenu = fileMenu->addMenu("&New Graph");
			fileNewMenu->addAction("&Blank Graph", gsApp, SLOT(addNewGraph()));

		fileMenu->addAction("&Load Graph...", this, SLOT(loadGraph()));
		saveGraphMenu = fileMenu->addMenu("&Save Graph");

		QAction* closeAct = fileMenu->addAction("&Close", this, SLOT(close()));
		closeAct->setShortcut(QKeySequence::Close);
		closeAct->setStatusTip(tr("Close the current GraphShop Window"));

		QAction* exitAct = fileMenu->addAction("E&xit", qApp, SLOT(closeAllWindows()));
		exitAct->setShortcut(QKeySequence::Quit);
		exitAct->setStatusTip(tr("Close all windows and exit GraphShop"));


	viewMenu = menubar->addMenu("&View");

		viewSeparator = viewMenu->addSeparator();

		QAction* viewScript = viewMenu->addAction("Script Editor");
		viewScript->setCheckable(true);
		//viewScript->setChecked(true);
		connect(viewScript, SIGNAL(triggered(bool)), SLOT(showScriptPanel(bool)));

		QAction* viewCode = viewMenu->addAction("Code Editor");
		viewCode->setCheckable(true);
		//viewCode->setChecked(true);
		connect(viewCode, SIGNAL(triggered(bool)), SLOT(showCodePanel(bool)));



	QMenu* windowMenu = menubar->addMenu("&Window");
		QAction* fullscreenAct = windowMenu->addAction("&Full Screen");
		fullscreenAct->setCheckable(true);
		fullscreenAct->setShortcut(Qt::Key_F11);
		connect(fullscreenAct, SIGNAL(triggered(bool)), SLOT(setFullScreen(bool)));

	setDockNestingEnabled(true);


	//|/connect(graphMenu, SIGNAL(graphSelected(Graph*)), this, SLOT(switchToGraph(Graph*)));
	connect(gsApp, SIGNAL(graphAdded(Graph*)), SLOT(addGraph(Graph*)));

	foreach(Graph* graph, gsApp->graphList())
	{
		addGraph(graph);
	}


	showCodePanel(true);
	showScriptPanel(true);
}



ScriptWindow* GraphShopWindow::showScriptPanel(bool show)
{
	if(!_scriptPanel)
	{
		_scriptPanel = new ScriptWindow(_graph, this);

		addDockWidget(Qt::BottomDockWidgetArea, _scriptPanel);

		if(_codePanel)
		{
			tabifyDockWidget(_scriptPanel, _codePanel);
		}
	}

	_scriptPanel->setVisible(show);
	_scriptPanel->raise();

	return _scriptPanel;
}

CodeWindow* GraphShopWindow::showCodePanel(bool show)
{
	if(!_codePanel)
	{
		_codePanel = new CodeWindow(_graph, this);

		addDockWidget(Qt::BottomDockWidgetArea, _codePanel);

		if(_scriptPanel)
		{
			tabifyDockWidget(_codePanel, _scriptPanel);
		}
	}

	_codePanel->setVisible(show);
	_codePanel->raise();

	return _codePanel;
}


void GraphShopWindow::addGraph(Graph *graph)
{
	GraphPack* pack = new GraphPack(graph, this);
	_graphpacks.insert(graph, pack);

	QMenu* panelMenu = pack->panelMenu();
	viewMenu->insertMenu(viewSeparator, panelMenu);
	panelMenu->menuAction()->setVisible(true);


	GraphAction* saveItem = new GraphAction(graph, true);
	connect(saveItem, SIGNAL(triggered(Graph*)), SLOT(saveGraph(Graph*)));

	saveGraphMenu->insertAction(0, saveItem);
}

void GraphShopWindow::setGraph(Graph *graph)
{
	//graphMenu->setGraph(graph);
}

void GraphShopWindow::loadGraph()
{
	QStringList filters;
	filters << "GraphShop Graph (*.gsg)";

	QFileDialog dialog(this);
	dialog.setNameFilters(filters);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::ExistingFile);

	if(dialog.exec())
	{
		QString fileName = dialog.selectedFiles()[0];
		QString filter = dialog.selectedNameFilter();

		if(filter == "GraphShop Graph (*.gsg)")
		{
			Graph* graph = new Graph(this);
			bool success = GraphFormatGSG::loadFile(graph, fileName);

			if(success)
			{
				gsApp->addGraph(graph);
			}
			else
			{
				QMessageBox::warning(this, "Load Error", "Unable to read or load file");
			}
		}
	}
}

void GraphShopWindow::saveGraph(Graph* graph)
{
	QStringList filters;
	filters << "GraphShop Graph (*.gsg)";

	QFileDialog dialog(this);
	dialog.setNameFilters(filters);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setFileMode(QFileDialog::AnyFile);

	if(dialog.exec())
	{
		QString fileName = dialog.selectedFiles()[0];
		QString filter = dialog.selectedNameFilter();

		if(filter == "GraphShop Graph (*.gsg)")
		{
			bool success = GraphFormatGSG::saveFile(graph, fileName);

			if(!success)
			{
				QMessageBox::warning(this, "Save Error", "Unable to save graph");
			}
		}
	}
}

void GraphShopWindow::switchToGraph(Graph *newgraph)
{
	GraphPack* oldpack = _graphpacks.value(_graph);
	GraphPack* newpack = _graphpacks.value(newgraph);

	if(oldpack)
	{
		//|/oldpack->panelMenu()->menuAction()->setVisible(false);
		oldpack->swapOut();
	}

	_graph = newgraph;
	setWindowTitle("GraphShop - "+_graph->label());

	//|/newpack->panelMenu()->menuAction()->setVisible(true);
	newpack->swapIn();

	emit graphSet(_graph);
}


void GraphShopWindow::setFullScreen(bool fullscreen)
{
	if(fullscreen)
	{
		_wasMaximized = isMaximized();
		showFullScreen();
	}
	else
	{
		if(_wasMaximized)
		{
			showMaximized();
		}
		else
		{
			showNormal();
		}
	}
}
