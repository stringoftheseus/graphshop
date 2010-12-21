#ifndef TOURNAMENTWINDOW_H
#define TOURNAMENTWINDOW_H

#include "tournamentdraw.h"
#include "windows/graphwindow.h"

class TournamentWindow : public GraphWindow
{
	Q_OBJECT

public:
	TournamentWindow(Graph* graph, GraphShopWindow* parent);
	TournamentWindow(Tournament* tournament, GraphShopWindow* parent);

protected:
	void _build();

private:
	Tournament* _tournament;
	TournamentDraw* draw;

private slots:
	void zoomIn();
	void zoomOut();
	void zoomAll();
	void addVertex();

	void updateTitle();
};

Q_DECLARE_METATYPE(TournamentWindow*)

#endif //TOURNAMENTVIEW_H
