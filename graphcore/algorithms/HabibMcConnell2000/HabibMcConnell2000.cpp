#include "graphcore/_graphcore_h.h"

#include "HabibMcConnell2000.h"

namespace HabibMcConnell2000 {


/** Algorithm 2, p63: Lex-BFS Ordering
 *
 * The implementation here is identical to that in the paper with the following exceptions:
 *
 *     1. The input is a component (a QList<Vertex*>) rather than a Graph. This is because the paper
 *        assumes that the given graph is connected, where in real life it often isn't. So to enable
 *        this to work (instead of blowing up) on unconnected graphs, we run the entire algorithm on
 *        each component of the graph we're given, then combine the resulting clique chains in order
 *        to generate the resulting interval graph.
 *
 *     2. In keeping with standard computer science tradition (and the rest of the GraphShop API) we
 *        use 0 to n-1 as the vertex indices in the ordering instead of 1 to n. Using 1 to n as done
 *        in the paper would require we either have an extra dummy element at index 0 of the list or
 *        use a QMap<int, Vertex*> instead of a QList<Vertex*>, both of which would complicate using
 *        the Lex-BFS ordering down the road.
 *
 *     3. We build the order directly as a QList of vertices, instead of making a map of vertices to
 *        integers first and then returning the inverse of that map as the order.
 *
 *     4. Because the algorithm fills the order from back to front, we prepopulate the order list to
 *        contain n null pointers at the beginnning so that we have all n slots to write to.
 */
QList<Vertex*> lexBFSOrder(QList<Vertex*> component)
{
	int n = component.length()-1;

	QList<Vertex*> Pi;
	for(int i=0; i<=n; i++) Pi.append(0);

	QList<QList<Vertex*> > L;
	L.append(component);


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
bool isChordal(QList<Vertex*> component, RNTree* t)
{
	// 0. If we don't have an RNTree yet, make one and start over...
	if(t == 0)
	{
		RNTree T(lexBFSOrder(component));
		return isChordal(component, &T);
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


/** Algorithm 4, p65: Clique Tree */
// Incorporated into the CliqueTree class constructor


/** Graph Component Splitter for Interval Graph Recognition
 *
 * This function doesn't actually do anything itself, other than call the other cliqueChain function
 * once for each component of the given graph, and return the combined result. The algorithm used to
 * find the clique chain (and thus the interval representation of the graph) assumes the given graph
 * is connected, whereas in real life it often isn't. This little wrapper is how we make it work for
 * all graphs in real life.
 *
 * If the algorithm fails to find a clique chain for any component of a graph, than the entire graph
 * is not an interval graph and we return an empty list.
 */
QList<QSet<Vertex*> > cliqueChain(Graph const* graph)
 {
	QList<QSet<Vertex*> > cliqueChain;
	foreach(QList<Vertex*> component, graph->getComponents())
	{
		QList<QSet<Vertex*> > cliques = HabibMcConnell2000::cliqueChain(component);

		if(!cliques.isEmpty())
		{
			cliqueChain.append(cliques);
		}
		else
		{
			return QList<QSet<Vertex*> >();
		}
	}

	return cliqueChain;
 }

/** Algorithm 9, p76: Interval Graph Recognition (via Clique Partition Refinement)
 *
 * If the given graph is found to be a valid interval graph, this function returns a clique chain of
 * the graph from which an interval representation can easily be created. If the graph isn't a valid
 * interval graph, the return value is an empty List.
 */
QList<QSet<Vertex*> > cliqueChain(QList<Vertex*> component)
{
	// 1. Compute a Lex-BFS ordering of the vertices in the graph
	QList<Vertex*> lexBFS = lexBFSOrder(component);

	// 2. Use the Lex-BFS ordering to ceate an RNTree for the graph
	RNTree rnTree(lexBFS);

	// 3. Use the RNTree to verify that the graph is chordal
	if(!isChordal(component, &rnTree))
	{
		// 3a. If it ain't chordal, it ain't interval (retrurn an empty list to signal false)
		return QList<QSet<Vertex*> >();
	}

	// 4. Use the RNTree to create a CliqueTree
	CliqueTree cliqueTree(&rnTree);

	// 5. Get the set of maximal cliques from the CliqueTree
	QList<QSet<Vertex*> > const maxCliques = cliqueTree.cliques();

	// 6. Create a list of lists of sets of vertices and seed it with the set of max cliques
	QList<QList<QSet<Vertex*> > > L;
	L.append(maxCliques);

	// 7. Initialize a set of vertices to function as the pivots stack (only not really a stack)
	QSet<Vertex*> pivots;

	// 8. Process cliques until L contains only singleton classes (i.e., contains n items)
	int n = maxCliques.length();
	QList<QSet<Vertex*> > xCliques;

	while(L.length() < n)
	{
		xCliques.empty();

		if(pivots.isEmpty())
		{
			// 8.1a.1. Find a nonsingleton class in L (a list with more than one element)
			int i;
			QList<QSet<Vertex*> > Xc;

			for(i=0; Xc.isEmpty(); i++)
			{
				if(L[i].length() > 1)
				{
					Xc = L[i];
				}
			}

			// 8.1a.2. Find the last clique in that class
			QList<QSet<Vertex*> > Cl;
			Cl.append(Xc.takeLast());

			// 8.1a.3. Replace Xc by Xc\{Cl},{Cl} in L
			L.replace(i-1, Xc);
			L.insert(i, Cl);

			// 8.1a.4. Set xCliques = {Cl}
			xCliques.append(Cl);
		}
		else
		{
			// 8.1b.1. Pick a pivot x from the pivots stack
			Vertex* x = pivots.toList()[0]; // it shouldn't be so hard to pop from a QSet, but it is
			pivots.remove(x);

			// 8.1b.2. Let xCliques be the set of all maximum cliques containing x
			foreach(QSet<Vertex*> clique, maxCliques)
			{
				if(clique.contains(x))
				{
					xCliques.append(clique);
				}
			}

			// 8.1b.3. Let Xa be the first element of L containing a member of xCliques
			int Ia;
			QList<QSet<Vertex*> > Xa;

			for(Ia=0; Xa.isEmpty(); Ia++)
			{
				QList<QSet<Vertex*> > Li = L[Ia];

				foreach(QSet<Vertex*> Liv, Li)
				{
					if(xCliques.contains(Liv))
					{
						Xa = Li;
						break;
					}
				}
			}

			// 8.1b.4. Replace Xa by Xa\xCliques,Xa inter xCliques in L
			QList<QSet<Vertex*> > XaM;
			QList<QSet<Vertex*> > XaI;

			foreach(QSet<Vertex*> clique, Xa)
			{
				if(xCliques.contains(clique))
				{
					XaI.append(clique);
				}
				else
				{
					XaM.append(clique);
				}
			}

			Ia--;
			L.removeAt(Ia);

			if(!XaM.isEmpty()) L.insert(Ia++, XaM);
			if(!XaI.isEmpty()) L.insert(Ia, XaI);

			// 8.1b.5. Let Xb be the last element of L containing a member of xCliques
			int Ib;
			QList<QSet<Vertex*> > Xb;

			for(Ib=L.length()-1; Xb.isEmpty(); Ib--)
			{
				QList<QSet<Vertex*> > Li = L[Ib];

				foreach(QSet<Vertex*> Liv, Li)
				{
					if(xCliques.contains(Liv))
					{
						Xb = Li;
						break;
					}
				}
			}

			// 8.1b.6. Replace Xb by Xb inter xCliques,Xb\xCliques in L
			QList<QSet<Vertex*> > XbM;
			QList<QSet<Vertex*> > XbI;

			foreach(QSet<Vertex*> clique, Xb)
			{
				if(xCliques.contains(clique))
				{
					XbI.append(clique);
				}
				else
				{
					XbM.append(clique);
				}
			}

			Ib++;
			L.removeAt(Ib);

			if(!XbI.isEmpty()) L.insert(Ib++, XbI);
			if(!XbM.isEmpty()) L.insert(Ib++, XbM);
		}

		// 8.2. Update the pivots based on "remaining" edges in the CliqueTree
		foreach(VertexNode* node, cliqueTree.nodes())
		{
			if(node->parentEdge)
			{
				QSet<Vertex*> Ci = node->vertices;
				QSet<Vertex*> Cj = node->parent->vertices;

				if(   (xCliques.contains(Ci) && !xCliques.contains(Cj))
				   || (xCliques.contains(Cj) && !xCliques.contains(Ci)))
				{
					pivots += Ci.intersect(Cj);

					node->parentEdge = false;
				}
			}
		}
	}

	// 9. Create the clique chain from L by converting singleton lists to bare sets
	QList<QSet<Vertex*> > cliqueChain;

	qDebug() << "Cliques:";
	foreach(QList<QSet<Vertex*> > cliqueList, L)
	{
		cliqueChain.append(cliqueList.first());

		QString cstring;
		foreach(Vertex* v, cliqueList.first())
		{
			cstring += QString::number(v->index()) + " ";
		}
		qDebug() << cstring;
	}
	qDebug() << '\n';

	// 10. Make sure the cliques containing each vertex are consecutive in L
	foreach(Vertex* x, component)
	{
		int i=0;

		// 10.1. Find the first clique containing x
		while(!cliqueChain[i].contains(x))
		{
			i++;
		}

		// 10.2. Find the first clique not containing x (if there is one)
		while(i < cliqueChain.length() && cliqueChain[i].contains(x))
		{
			i++;
		}

		// 10.3 Make sure none of the rest of the cliques contain x
		for(; i<cliqueChain.length(); i++)
		{
			if(cliqueChain[i].contains(x))
			{
				// 10.3a. This isn't a valid clique chain, so it isn't an interval graph
				return QList<QSet<Vertex*> >();
			}
		}
	}

	// 11. All the vertices are consecutive in the clique chain! We're done!
	return cliqueChain;
}


} // namespace HabibMcConnell2000
