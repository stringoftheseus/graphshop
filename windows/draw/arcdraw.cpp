#include <math.h>

#include "graphcore/_graphcore_h.h"

#include "vertexdraw.h"
#include "arcdraw.h"


ArcDraw::ArcDraw(VertexDraw *tail, VertexDraw *head): LineDraw(tail, head, true), arc(0)
{

}

ArcDraw::ArcDraw(DrawTables *tables, Arc *arc): LineDraw(tables->V.value(arc->tail()),
														 tables->V.value(arc->head()), true),
												arc(arc)
{
}

Arc* ArcDraw::getArc()
{
	return arc;
}

int ArcDraw::index() const
{
	int level;

	if(!_tail->getVertex()->arcsWith(_head->getVertex()).contains(arc))
	{
		level = 0;
	}
	else if(_head->getVertex()->index() > _tail->getVertex()->index())
	{
		level = (  _tail->getVertex()->edgesWith(_head->getVertex()).size()
				+ _tail->getVertex()->arcsWith(_head->getVertex()).indexOf(arc));
	}
	else
	{
		level = (  _head->getVertex()->edgesWith(_tail->getVertex()).size()
				+ _head->getVertex()->arcsWith(_tail->getVertex()).indexOf(arc));
	}

	return level;
}
