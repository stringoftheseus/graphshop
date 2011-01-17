#include "graphcore/_graphcore_h.h"
#include "rntree.h"

namespace HabibMcConnell2000 {

/** RN Tree Constructure
 * from Algorithm 3, p64: Chordality Test
 *
 * Creates the tree T based on a Lex-BFS ordering of the vertices of a graph such as returned by the
 * HabibMcConnell2000::lexBFSOrder() function. The tree construction algorithm was written as a part
 * of Algorithm 3 (Chordality test) but moved into its own class so that it wouldn't have to also be
 * written in Algorithm 4 (Clique tree) which uses the same structure.
 */
RNTree::RNTree(QList<Vertex*> pi)
{
	// 1. Construct a VertexNode for each vertex in pi, and keep a map of Vertex->Node
	QHash<Vertex*, VertexNode*> nodes;

	foreach(Vertex* vertex, pi)
	{
		VertexNode* node = new VertexNode();
		node->vertex = vertex;

		// This is used down in 2.4a.1. to find the node associated with a given vertex
		nodes.insert(vertex, node);
	}

	// 2. Calculate and store RN(x) and parent(x) for each node x; note which node is the root
	foreach(VertexNode* node, nodes)
	{
		// 2.1. Save this node's vertex's index so we can look for nodes to its right
		int me = pi.indexOf(node->vertex);

		// 2.2. Dummpy parent value higher than any valid index in pi (makes the qMin calls work)
		int parent = pi.length();

		// 2.3. Check each vertex adjacent to the current node's vertex
		foreach(Vertex* vertex, node->vertex->neighborhood())
		{
			int you = pi.indexOf(vertex);

			// A greater index means this node is a neighbor to the right--i.e., in RN(x)
			if(you > me)
			{
				node->vertices.insert(vertex);

				// Keep track of the "leftmost" (lowest indexed) RN node, which will be parent(x)
				parent = qMin(parent, you);
			}
		}

		// 2.4. Correctly set this node's parent node, or mark it as the root if it has none
		if(node->vertices.isEmpty() == false)
		{
			// 2.4a.1. Set the correct node as this one's parent
			node->parent = nodes.value(pi.at(parent));

			// 2.4a.2. Add this node to the parent node's child set
			node->parent->children.insert(node);
		}
		else
		{
			// 2.4b.1. This node has no parent node...
			node->parent = 0;

			// 2.4b.1. ...which must mean it's the root of the tree
			_root = node;
		}

	}
}


QList<VertexNode const*> RNTree::preorderNodes() const
{
	return preorderNodes(_root);
}

QList<VertexNode const*> RNTree::preorderNodes(const VertexNode *) const
{
	QList<VertexNode const*> order;

	order.append(node);

	foreach(VertexNode* child, node->children)
	{
		order.append(preorderNodes(child));
	}

	return order;
}


QList<VertexNode const*> RNTree::postorderNodes() const
{
	return postorderNodes(_root);
}

QList<VertexNode const*> RNTree::postorderNodes(const VertexNode* node) const
{
	QList<VertexNode const*> order;

	foreach(VertexNode* child, node->children)
	{
		order.append(postorderNodes(child));
	}

	order.append(node);

	return order;
}



RNTree::~RNTree()
{
	// Recursively delete all nodes in the tree starting with the root
	deleteNode(_root);
}

void RNTree::deleteNode(VertexNode* node)
{
	// 1. Recursively delete all children of this node
	foreach(VertexNode* child, node->children)
	{
		deleteNode(child);
	}

	// 2. Delete this node
	delete node;
}

} // namespace HabibMcConnell2000
