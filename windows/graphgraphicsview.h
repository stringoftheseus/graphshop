#ifndef GRAPHGRAPHICSVIEW_H
#define GRAPHGRAPHICSVIEW_H

#include <QtGui>
#include <QGraphicsView>

class GraphGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphGraphicsView(QWidget *parent = 0);

	QAction* exportAction();

protected:

private slots:
	void doExport();

private:
	QAction* _exportAction;

};

#endif // GRAPHGRAPHICSVIEW_H
