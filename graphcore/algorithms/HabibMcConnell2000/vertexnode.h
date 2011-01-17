#ifndef VERTEXNODE_H
#define VERTEXNODE_H

#include "graphcore/_graphcore_c.h"

namespace HabibMcConnell2000 {

struct VertexNode
{
	bool parentEdge; // Used by CliqueTree to "remove" an edge from the tree

	VertexNode* parent;
	QSet<VertexNode*> children;

	Vertex* vertex;
	QSet<Vertex*> vertices;
};

} // namespace HabibMcConnell2000

#endif // VERTEXNODE_H
