#include "graphcore/_graphcore_h.h"

#include "HabibMcConnell2000.h"

namespace HabibMcConnell2000 {


/** Algorithm 2, p63: Lex-BFS Ordering
 *
 * The implementation here is identical to that in the paper with the following exceptions:
 *
 *     1. In keeping with standard computer science tradition (and the rest of the GraphShop API) we
 *        use 0 to n-1 as the vertex indices in the ordering instead of 1 to n. Using 1 to n as done
 *        in the paper would require we either have an extra dummy element at index 0 of the list or
 *        use a QMap<int, Vertex*> instead of a QList<Vertex*>, both of which would complicate using
 *        the Lex-BFS ordering down the road.
 *
 *     2. We build the order directly as a QList of vertices, instead of making a map of vertices to
 *        integers first and then returning the inverse of that map as the order.
 *
 *     3. Because the algorithm fills the order from back to front, we prepopulate the order list to
 *        contain n null pointers at the beginnning so that we have all n slots to write to.
 */
QList<Vertex*> lexBFSOrder(Graph const* graph)
{
	int n = graph->vertexCount()-1;

	QList<Vertex*> Pi;
	for(int i=0; i<=n; i++) Pi.append(0);

	QList<QList<Vertex*> > L;
	L.append(graph->getVertexSet());


	for(int i = n; i>=0; i--)
	{
		QList<Vertex*> Llast = L.last();
		Vertex* x = Llast.takeLast();

		if(Llast.isEmpty())
		{
			L.removeLast();
		}
		else
		{
			L.replace(L.length()-1, Llast);
		}


		Pi.replace(i, x);

		for(int b=0; b<L.length(); b++)
		{
			QList<Vertex*> Xb = L[b];
			QList<Vertex*> Y;

			foreach(Vertex* y, Xb)
			{
				if(y->neighborhood().contains(x))
				{
					Xb.removeAll(y);
					Y.append(y);
				}
			}

			if(Y.isEmpty() == false && Xb.isEmpty() == false)
			{
				L.replace(b, Xb);
				L.insert(b+1, Y);
			}
		}
	}

	return Pi;
}



/** Algorithm 3, p64: Chordality Test
 *
 * The algorithm used here is the same as that in the algorithm, except that the actual construction
 * and management of the tree T has been moved into the RNTree class. For that reason, the input has
 * been modified to include an RNTree should the caller wish to provide one. This allows Algorithm 4
 * (Clique Tree) to construct a single tree and have it used here and there, instead of having to do
 * the tree setup calculations twice.
 */
bool isChordal(Graph const* graph, RNTree* t)
{
	// 0. If we don't have an RNTree yet, make one and start over...
	if(t == 0)
	{
		RNTree T(lexBFSOrder(graph));
		return isChordal(graph, &T);
	}

	// 1. The part where we compute the tree t is in the RNTree constructor (rntree.cpp)

	// 2. For each nonroot node in t, verify that RN(x)\parent(x) is a subset of RN(parent(x))
	foreach(VertexNode const* node, t->postorderNodes())
	{
		// RN(x)-RN(parent(x)) should contain 1 vertex only (parent(x)); more than that is bad
		if(node->parent && (node->vertices - node->parent->vertices).size() > 1)
		{
			// 2.1. Since the above check filaed, we know that this is *not* a chordal graph
			return false;
		}
	}

	// 3. The above check passed for every vertex, so this *is* a chordal graph
	return true;
}


/* Algorithm 4, p65: Clique Tree */
// Incorporated into the CliqueTree class constructor


/* Algorithm 9, p76: Interval Graph Recognition (via Clique Partition Refinement) */
QList<QSet<Vertex*> > cliqueChain(Graph const* graph)
{

}


} // namespace HabibMcConnell2000
