#include "cliquetree.h"

namespace HabibMcConnell2000 {

CliqueTree::CliqueTree(RNTree const* t)
{
	QMap<VertexNode*, VertexNode*> cliques;

	QList<VertexNode const*> nodes = preorderNodes();
	nodes.removeFirst();
	
	foreach(VertexNode const* node, nodes)
	{
		if(node->vertices - node->parent->vertex == node->parent->vertices)
		{
			cliques[node->parent]->vertices.insert(node->vertex);
			cliques.insert(node, cliques[node->parent]);
		}
		else
		{
			VertexNode* C = new VertexNode();
			C->vertices.insert(node->vertex);			
			
			if(cliques.contains(node->parent)
			{
				C.parent = cliques[node->parent];
			}
			else
			{
				C.parent = 0;
				_root = C;
			}
			 
			cliques.insert(node, C);
		}
	}
	
	_nodes = cliques.values();
}

CliqueTree::~CliqueTree()
{
	foreach(VertexNode* node, _nodes_
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
