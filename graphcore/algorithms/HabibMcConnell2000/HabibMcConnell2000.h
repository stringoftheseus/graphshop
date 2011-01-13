#ifndef HabibMcConnell2000_H
#define HabibMcConnell2000_H

#include "graphcore/_graphcore_c.h"
#include "rntree.h"

namespace HabibMcConnell2000
{
	/* Algorithm 2, p63: Lex-BFS Ordering */
	QList<Vertex*> lexBFSOrder(Graph const* graph);

	/* Algorithm 3, p64: Chordality Test */
	bool isChordal(Graph const* graph, RNTree* t=0);

	/* Algorithm 4, p65: Clique Tree */
	// Incorporated into the CliqueTree class constructor

	/* Algorithm 9, p76: Interval Graph Recognition (via Clique Partition Refinement) */
	QList<QSet<Vertex*> > cliqueChain(Graph const* graph);
}



#endif // HabibMcConnell2000_H
