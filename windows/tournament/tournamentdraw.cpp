#include <QKeyEvent>
#include <QContextMenuEvent>


#include "tournamentdraw.h"
#include "tournamentvertexdraw.h"
#include "tournamentarcdraw.h"

TournamentDraw::TournamentDraw(Tournament *tournament): tournament(tournament)
{
	//setMinimumSize(200, 200);

	scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setBackgroundBrush(palette().background());

	//scene->setSceneRect(-100, -100, 100, 100);
	setScene(scene);

	setViewportUpdateMode(FullViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorViewCenter);
	setResizeAnchor(AnchorViewCenter);

	//setMode(SELECT);

	setCursor(Qt::ArrowCursor);
	setInteractive(true);
	setMouseTracking(false);
	setDragMode(RubberBandDrag);

	scene->clear();

	QFont invalidFont;
	invalidFont.setPixelSize(20);
	invalidFont.setBold(true);
	invalidNotice = scene->addText("Invalid Tournament", invalidFont);

	if(tournament->valid())
	{
		scene->removeItem(invalidNotice);
	}

	scoreDraw = 0;

	for(int i=0; i<tournament->order(); i++)
	{
		vertexAdded(i);
	}

	connect(tournament, SIGNAL(arcFlipped(int,int)), this, SLOT(arcFlipped(int,int)));

	connect(tournament, SIGNAL(vertexAdded(int)),   this, SLOT(vertexAdded(int)));
	connect(tournament, SIGNAL(vertexDeleted(int)), this, SLOT(vertexDeleted(int)));

	connect(tournament, SIGNAL(cleared()), this, SLOT(cleared()));
	connect(tournament, SIGNAL(validityChanged(bool)), this, SLOT(validity(bool)));
}

TournamentDraw::~TournamentDraw()
{

}



QSize TournamentDraw::sizeHint() const
{
	return QSize(300, 300);
}

void TournamentDraw::contextMenuEvent(QContextMenuEvent *event)
{
	foreach(QGraphicsItem* item, items(event->pos()))
	{
		if(!item->isSelected())
		{
			scene->clearSelection();
			break;
		}
	}

	foreach(QGraphicsItem* item, items(event->pos()))
	{
		item->setSelected(true);
	}


	/* TODO: Make this menu enable and disable the different options based on the
			 content of the current selection */

	QMenu *contextMenu = new QMenu();

	contextMenu->addAction("Flip Arcs", this, SLOT(flipSelectedArcs()));
	contextMenu->addSeparator();
	contextMenu->addAction("Delete All", this, SLOT(deleteSelection()));
	contextMenu->exec(event->globalPos());
	delete contextMenu;
}

void TournamentDraw::enterEvent(QEvent *event)
{/*
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		//ghostVertex = new VertexDraw();
		scene->addItem(ghostVertex);
		ghostVertex->setSelected(true);

		ghostVertex->setPos(mapToScene(mapFromGlobal(QCursor::pos())));

		if(mode == ADD_EDGE)
		{
			scene->addItem(ghostEdge);
		}
		else if(mode == ADD_ARC)
		{
			scene->addItem(ghostArc);
		}
	}
*/
	QGraphicsView::enterEvent(event);
}

void TournamentDraw::leaveEvent(QEvent *event)
{/*
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		scene->removeItem(ghostVertex);
		//delete ghostVertex;

		if(mode == ADD_EDGE)
		{
			scene->removeItem(ghostEdge);
		}
		else if(mode == ADD_ARC)
		{
			scene->removeItem(ghostArc);
		}
	}
*/

	QGraphicsView::leaveEvent(event);
}

void TournamentDraw::mouseMoveEvent(QMouseEvent *event)
{/*
	if(mode == ADD_VERTEX || mode == ADD_EDGE || mode == ADD_ARC)
	{
		ghostVertex->setVisible(true);
		VertexDraw* hover = getHoveredVertex();

		if(hover)
		{
			hover->setSelected(true);
			ghostVertex->setVisible(false);
		}
		else
		{
			scene->clearSelection();
			ghostVertex->setSelected(true);
		}

		ghostVertex->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
	}
*/
	QGraphicsView::mouseMoveEvent(event);
}

void TournamentDraw::mousePressEvent(QMouseEvent *event)
{/*
	if(mode == ADD_VERTEX)
	{
		graph->addVertex();
	}
	else if(mode == ADD_EDGE || mode == ADD_ARC)
	{
		if((mode == ADD_EDGE && ghostEdge) || (mode == ADD_ARC && ghostArc))
		{
			VertexDraw* vd2 = getHoveredVertex();

			if(vd2 == 0)
			{
				Vertex* v2 = graph->addVertex();
				vd2 = tables.V[v2];
				vd2->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
			}

			if(mode == ADD_EDGE)
			{
				graph->addEdge(ghostEdge->getV1()->getVertex(), vd2->getVertex());

				scene->removeItem(ghostEdge);
				delete ghostEdge;
				ghostEdge = 0;
			}
			else
			{
				graph->addArc(ghostArc->getTail()->getVertex(), vd2->getVertex());

				scene->removeItem(ghostArc);
				delete ghostArc;
				ghostArc = 0;
			}
		}
		else
		{
			VertexDraw* vd1 = getHoveredVertex();

			if(vd1 == 0)
			{
				Vertex* v1 = graph->addVertex();

				vd1 = tables.V[v1];
				vd1->setPos(mapToScene(mapFromGlobal(QCursor::pos())));
			}

			if(mode == ADD_EDGE)
			{
				ghostEdge = new EdgeDraw(vd1, ghostVertex);
				scene->addItem(ghostEdge);
			}
			else
			{
				ghostArc = new ArcDraw(vd1, ghostVertex);
				scene->addItem(ghostArc);
			}
		}
	}
*/
	QGraphicsView::mousePressEvent(event);
}

void TournamentDraw::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
/*
	foreach(QGraphicsItem* item, items())
	{
		item->update();
	}*/
}

void TournamentDraw::dragMoveEvent(QDragMoveEvent *event)
{
	QGraphicsView::dragMoveEvent(event);
}

void TournamentDraw::addArcDraw(int t, int h)
{
	TournamentArcDraw* ad = new TournamentArcDraw(vertices[t], vertices[h]);
	scene->addItem(ad);
}

void TournamentDraw::arcFlipped(int v1, int v2)
{
	vertices[v1]->getArc(vertices[v2])->flip();
	updateScore();
}

void TournamentDraw::vertexAdded(int index)
{
	TournamentVertexDraw* vd = new TournamentVertexDraw(&vertices);
	scene->addItem(vd);

	for(int i=0; i<index; i++)
	{
		TournamentArcDraw* ad;

		if(tournament->hasArc(i, index))
		{
			ad = new TournamentArcDraw(vertices[i], vd);
		}
		else
		{
			ad = new TournamentArcDraw(vd, vertices[i]);
		}

		scene->addItem(ad);
	}

	updateScore();
}

void TournamentDraw::vertexDeleted(int index)
{
	TournamentVertexDraw* vd = vertices[index];
	scene->removeItem(vd);

	vertices.removeAll(vd);

	for(int v=0; v<vertices.count(); v++)
	{
		TournamentVertexDraw* vertex = vertices[v];
		TournamentArcDraw* ad = vertex->getArc(vd);
		scene->removeItem(ad);

		vertex->removeArc(vd);
		vertex->update();
	}

	updateScore();
}

void TournamentDraw::cleared()
{
	scene->clear();

	vertices.clear();
}

void TournamentDraw::validity(bool valid)
{
	setEnabled(valid);

	if(valid)
	{
		scene->removeItem(invalidNotice);

		for(int i=0; i<vertices.count(); i++)
		{
			for(int j=i+1; j<vertices.count(); j++)
			{
				TournamentArcDraw* ad = vertices[i]->getArc(vertices[j]);

				if(tournament->hasArc(ad->tail()->index(), ad->head()->index()) == false)
				{
					ad->flip();
				}
			}
		}
	}
	else
	{
		scene->addItem(invalidNotice);
	}
}

void TournamentDraw::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Delete)
	{
		deleteSelection();
	}
}

TournamentVertexDraw* TournamentDraw::getHoveredVertex()
{
	foreach(TournamentVertexDraw* vd, vertices)
	{
		if(vd->isUnderMouse())
		{
			return vd;
		}
	}

	return 0;
}

void TournamentDraw::deleteSelection()
{
	foreach(TournamentVertexDraw* vd, vertices)
	{
		if(vd->isSelected())
		{
			tournament->removeVertex(vd->index());
		}
	}
}

void TournamentDraw::flipSelectedArcs()
{
	for(int i=0; i<vertices.count(); i++)
	{
		for(int j=i+1; j<vertices.count(); j++)
		{
			if(vertices[i]->getArc(vertices[j])->isSelected())
			{
				tournament->flipArc(i, j);
			}
		}
	}
}

void TournamentDraw::viewAll()
{
	fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void TournamentDraw::addVertex()
{
	tournament->addVertex();
}

void TournamentDraw::updateScore()
{
	QList<int> score = tournament->scoreSequence();
	QStringList scorestring;

	foreach(int item, score)
	{
		scorestring << QString::number(item);
	}

	if(scoreDraw)
	{
		scene->removeItem(scoreDraw);
	}

	scoreDraw = scene->addText("Score: [" + scorestring.join(", ") + "]");
	scoreDraw->setPos(-14, 15);
}
