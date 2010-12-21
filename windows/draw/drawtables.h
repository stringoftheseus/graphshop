#ifndef DRAWTABLES_H
#define DRAWTABLES_H

#include <QtCore>

class Vertex;
class Edge;
class Arc;

class VertexDraw;
class EdgeDraw;
class ArcDraw;

struct DrawTables {
		QMap<Vertex*, VertexDraw*> V;
		QMap<Edge*, EdgeDraw*> E;
		QMap<Arc*, ArcDraw*> A;
};

#endif // DRAWTABLES_H
