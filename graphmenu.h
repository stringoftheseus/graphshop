#ifndef GRAPHMENU_H
#define GRAPHMENU_H

#include <QMenu>
#include "graphcore/_graphcore_c.h"
#include "graphaction.h"

class GraphMenu : public QMenu
{
    Q_OBJECT

public:
	explicit GraphMenu(QWidget* parent);

public slots:
	GraphAction* addGraph(Graph*);
	void setGraph(Graph*);

	void cycleNext();
	void cyclePrev();

private:
	QMap<Graph*, GraphAction*> actionMap;

	QActionGroup* _group;

signals:
	void graphSelected(Graph*);

};

#endif // GRAPHMENU_H
