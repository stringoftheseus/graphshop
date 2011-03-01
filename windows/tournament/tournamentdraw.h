#ifndef tournamentdraw_H
#define tournamentdraw_H

#include "graphcore/representations/tournament.h"

#include "windows/graphgraphicsview.h"
#include "tournamentvertexdraw.h"

class TournamentDraw : public GraphGraphicsView
{
	Q_OBJECT

public:
	TournamentDraw(Tournament* tournament);
	~TournamentDraw();
/*
	enum InteractionMode {SELECT, PAN, ADD_VERTEX, ADD_ARC};
	InteractionMode getMode();
	void setMode(InteractionMode);

	void setGraph(Graph*);
	Graph* getGraph();
*/
public slots:
	void viewAll();
	void addVertex();

protected:
	QSize sizeHint() const;
	void contextMenuEvent(QContextMenuEvent* event);

	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void mouseMoveEvent(QMouseEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);

private:
	void addArcDraw(int head, int tail);

	void keyPressEvent(QKeyEvent *event);

	TournamentVertexDraw* getHoveredVertex();

	/*InteractionMode mode;*/

	Tournament* tournament;

	QGraphicsScene *scene;
	QGraphicsItem* invalidNotice;

	QGraphicsItem* scoreDraw;
	void updateScore();

	QList<TournamentVertexDraw*> vertices;

private slots:
	void deleteSelection();
	void flipSelectedArcs();

	void arcFlipped(int, int);

	void vertexAdded(int);
	void vertexDeleted(int);

	void cleared();
	void validity(bool);

};

#endif // tournament_H
