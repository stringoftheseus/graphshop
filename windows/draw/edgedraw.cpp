#include <math.h>

#include <QPen>
#include <QBrush>
#include <QPainter>

#include "graphcore/graph.h"

#include "vertexdraw.h"
#include "edgedraw.h"


EdgeDraw::EdgeDraw(VertexDraw *v1, VertexDraw *v2): LineDraw(v1, v2, false), edge(0)
{

}

EdgeDraw::EdgeDraw(DrawTables *tables, Edge *edge): LineDraw(tables->V.value(edge->vertex1()),
															 tables->V.value(edge->vertex2()), false),
													edge(edge)
{
}

Edge* EdgeDraw::getEdge()
{
	return edge;
}

int EdgeDraw::index() const
{
	// TODO: this is an ugly hack and should be cleaned up, but right now I just need it working
	EdgeDraw* me = const_cast<EdgeDraw*>(this);

	return me->tail()->getVertex()->edgesWith(me->head()->getVertex()).indexOf(edge);
}
