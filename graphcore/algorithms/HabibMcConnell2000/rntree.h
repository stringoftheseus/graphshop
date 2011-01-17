#ifndef HabibMcConnell2000_RNTREE_H
#define HabibMcConnell2000_RNTREE_H

#include "graphcore/_graphcore_c.h"
#include "vertexnode.h"

namespace HabibMcConnell2000 {

class RNTree
{
public:
	RNTree(QList<Vertex*> lexBFSOrder);
	~RNTree();

	QList<VertexNode const*>  preorderNodes() const;
	QList<VertexNode const*> postorderNodes() const;
private:
	VertexNode* _root;

	QList<VertexNode const*> postorderNodes(VertexNode const*) const;

	void deleteNode(VertexNode*);

};

} // namespace HabibMcConnell2000

#endif // HabibMcConnell2000_RNTREE_H
