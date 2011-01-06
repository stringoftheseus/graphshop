#include "graphshopwindow.h"

#include "graphcore/_graphcore_h.h"
#include "graphcore/operations/_graphoperations_h.h"
#include "windows/_panels_h.h"

#include "graphpack.h"

GraphPack::GraphPack(Graph *graph, GraphShopWindow *parent) : QObject(parent), _graph(graph), _parent(parent)
{
	built = false;

	_panelMenu = new QMenu(graph->label(), _parent);
	connect(_panelMenu, SIGNAL(aboutToShow()), SLOT(build()));
}

void GraphPack::build()
{
	if(!built)
	{
		_adjMatrixWindow = new AdjMatrixWindow(_graph, _parent);
		_panelMenu->addAction(_adjMatrixWindow->toggleViewAction());

		_drawWindow      = new DrawWindow(_graph, _parent);
		_panelMenu->addAction(_drawWindow->toggleViewAction());

		_intervalGraphWindow = new IntervalGraphWindow(_graph, _parent);
		_panelMenu->addAction(_intervalGraphWindow->toggleViewAction());

		_tournamentWindow = new TournamentWindow(_graph, _parent);
		_panelMenu->addAction(_tournamentWindow->toggleViewAction());


		_panelMenu->addSeparator();

		_competitionGraph = new GraphPack(new CompetitionGraph(_graph), _parent);
		_panelMenu->addMenu(_competitionGraph->panelMenu());

		_complementGraph = new GraphPack(new ComplementGraph(_graph), _parent);
		_panelMenu->addMenu(_complementGraph->panelMenu());

		_converseGraph = new GraphPack(new ConverseGraph(_graph), _parent);
		_panelMenu->addMenu(_converseGraph->panelMenu());

		_dominationGraph = new GraphPack(new DominationGraph(_graph), _parent);
		_panelMenu->addMenu(_dominationGraph->panelMenu());

		_underlyingGraph = new GraphPack(new UnderlyingGraph(_graph), _parent);
		_panelMenu->addMenu(_underlyingGraph->panelMenu());


		built = true;
	}
}

QMenu* GraphPack::panelMenu()
{
	return _panelMenu;
}

void GraphPack::swapIn()
{
	build();

	if(_panelState.length() > 0)
	{
		_parent->restoreState(_panelState);
	}
}

void GraphPack::swapOut()
{
	_panelState = _parent->saveState();

	hidePanels();
}

void GraphPack::hidePanels()
{
	if(built)
	{
		_adjMatrixWindow->hide();
		_drawWindow->hide();
		_tournamentWindow->hide();

		_competitionGraph->hidePanels();
		_complementGraph->hidePanels();
		_converseGraph->hidePanels();
		_dominationGraph->hidePanels();
		_underlyingGraph->hidePanels();
	}
}
