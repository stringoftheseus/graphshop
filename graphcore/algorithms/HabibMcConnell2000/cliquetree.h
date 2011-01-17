#ifndef HabibMcConnell2000_CLIQUETREE_H
#define HabibMcConnell2000_CLIQUETREE_H

#include "graphcore/_graphcore_c.h"
#include "rntree.h"

namespace HabibMcConnell2000 {

class CliqueTree
{
public:
	CliqueTree(RNTree const*);
	~CliqueTree();

	QList<QSet<Vertex*> > cliques();

	QList<VertexNode*> nodes();

protected:
	VertexNode* _root;

	/* The order in which cliques are "discovered" is important
	to the algorithm, so this is a list not a set. */
	QList<VertexNode*> _nodes;
};

} // namespace HabibMcConnell2000

#endif // HabibMcConnell2000_CLIQUETREE_H
