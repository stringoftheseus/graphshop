#ifndef arcdraw_h
#define arcdraw_h

#include "linedraw.h"

class ArcDraw: public LineDraw
{
public:
	ArcDraw(VertexDraw *tail, VertexDraw *head);
	ArcDraw(DrawTables* drawtables, Arc* arc);

	Arc* getArc();

protected:
	int index() const;

private:
	Arc* const arc;

	friend class GraphDraw;
};

#endif // arcdraw_h
