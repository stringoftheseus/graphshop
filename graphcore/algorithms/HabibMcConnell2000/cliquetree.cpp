#include "cliquetree.h"

namespace HabibMcConnell2000 {

CliqueTree::CliqueTree(RNTree const* t)
{
	QMap<VertexNode const*, VertexNode*> cliques;

	QList<VertexNode const*> nodes = t->preorderNodes();
	//nodes.removeFirst();
	
	foreach(VertexNode const* node, nodes)
	{
		QSet<Vertex*> RNx = node->vertices;
		if(node->parent)
		{
			RNx.remove(node->parent->vertex);
		}

		if(node->parent && RNx == node->parent->vertices)
		{
			cliques[node->parent]->vertices.insert(node->vertex);
			cliques.insert(node, cliques[node->parent]);
		}
		else
		{
			VertexNode* C = new VertexNode();
			C->vertices.insert(node->vertex);
			C->vertices.unite(node->vertices); // this line isn't in the algorithm
			
			if(cliques.contains(node->parent))
			{
				C->parent = cliques[node->parent];
				C->parentEdge = true;
			}
			else
			{
				C->parent = 0;
				C->parentEdge = false;
				_root = C;
			}
			 
			cliques.insert(node, C);
			_nodes.append(C);
		}
	}
}

CliqueTree::~CliqueTree()
{
	foreach(VertexNode* node, _nodes)
	{
		delete node;
	}
}

QList<QSet<Vertex*> > CliqueTree::cliques()
{
	QList<QSet<Vertex*> > cliqueList;
	
	foreach(VertexNode* node, _nodes)
	{
		cliqueList.append(node->vertices);
	}
	
	return cliqueList;
}

QList<VertexNode*> CliqueTree::nodes()
{
	return _nodes;
}


} // namespace HabibMcConnell2000
