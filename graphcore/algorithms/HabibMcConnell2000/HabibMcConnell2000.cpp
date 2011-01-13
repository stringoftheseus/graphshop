#include "graphcore/_graphcore_h.h"

#include "HabibMcConnell2000.h"

namespace HabibMcConnell2000
{
	/* Algorithm 2, p63: Lex-BFS Ordering */
	QList<Vertex*> lexBFSOrder(Graph const* graph)
	{

	}


	/* Algorithm 3, p64: Chordality Test */
	bool isChordal(Graph const* graph, RNTree* t)
	{
		if(t == 0)
		{
			return isChordal(graph, &RNTree(lexBFSOrder(graph)));
		}

		// The part where we compute T is done in the RNTree constructor
	}


	/* Algorithm 4, p65: Clique Tree */
	// Incorporated into the CliqueTree class constructor


	/* Algorithm 9, p76: Interval Graph Recognition (via Clique Partition Refinement) */
	QList<QSet<Vertex*> > cliqueChain(Graph const* graph)
	{

	}

}
