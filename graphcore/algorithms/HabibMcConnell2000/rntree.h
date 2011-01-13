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

	QList<VertexNode const*> const preorderNodes();
	QList<VertexNode const*> const postorderNodes();
private:
	VertexNode* _root;

	void deleteNode(VertexNode*);

};

} // namespace HabibMcConnell2000

#endif // HabibMcConnell2000_RNTREE_H
