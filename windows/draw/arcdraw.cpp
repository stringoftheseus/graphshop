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
// TODO: this is an ugly hack and should be cleaned up, but right now I just need it working
	ArcDraw* me = const_cast<ArcDraw*>(this);

	return (  me->tail()->getVertex()->edgesWith(me->head()->getVertex()).size()
			+ me->tail()->getVertex()->arcsWith(me->head()->getVertex()).indexOf(arc));
}
