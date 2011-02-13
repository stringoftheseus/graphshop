#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDockWidget>

#include "graphcore/graph.h"
#include "graphshopwindow.h"

class GraphWindow: public QDockWidget
{
	Q_OBJECT

public:
	GraphWindow(GraphShopWindow* parent);
	void build();

protected:
	QToolBar* _toolbar;

	void setTitle(QString);

protected slots:
	void updateWindowTitle();
	void handleVisibilityChange(bool);

private:

	bool _built;
	virtual void _build() = 0;

	QString _title;
};

#endif // GRAPHWINDOW_H
