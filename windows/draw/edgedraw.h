#ifndef EDGEDRAW_H
#define EDGEDRAW_H

#include "linedraw.h"

class EdgeDraw: public LineDraw
{
public:
	EdgeDraw(VertexDraw* v1, VertexDraw* v2);
	EdgeDraw(DrawTables* drawtables, Edge* edge);

	Edge* getEdge();

protected:
	int index() const;

private:
	Edge* const edge;
};

#endif // EDGEDRAW_H
