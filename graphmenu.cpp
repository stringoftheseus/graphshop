#include <QShortcut>

#include "graphshopapp.h"

#include "graphmenu.h"


GraphMenu::GraphMenu(QWidget* parent) : QMenu("&Graph", 0)
{
	_group = new QActionGroup(this);

	connect(new QShortcut(Qt::CTRL | Qt::Key_Tab, parent),             SIGNAL(activated()), SLOT(cycleNext()));
	connect(new QShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Tab, parent), SIGNAL(activated()), SLOT(cyclePrev()));
}

GraphAction* GraphMenu::addGraph(Graph *graph)
{
	GraphAction* graphAct = new GraphAction(graph);
	connect(graphAct, SIGNAL(triggered(Graph*)), this, SIGNAL(graphSelected(Graph*)));

	graphAct->setCheckable(true);

	_group->addAction(graphAct);
	addAction(graphAct);
	actionMap.insert(graph, graphAct);

	if(_group->checkedAction() == 0)
	{
		graphAct->trigger();
	}

	return graphAct;
}

void GraphMenu::setGraph(Graph *graph)
{
	gsApp->includeGraph(graph);

	GraphAction* action = actionMap.value(graph);

	if(action->isChecked() == false)
	{
		action->trigger();
	}
}

void GraphMenu::cyclePrev()
{
	_group->actions().at((actions().indexOf(_group->checkedAction())+actions().length()-1) % actions().length())->trigger();
}

void GraphMenu::cycleNext()
{
	_group->actions().at((actions().indexOf(_group->checkedAction())+1) % actions().length())->trigger();
}
