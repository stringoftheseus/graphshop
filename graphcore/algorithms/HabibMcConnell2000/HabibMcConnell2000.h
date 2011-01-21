#ifndef HabibMcConnell2000_H
#define HabibMcConnell2000_H

#include "graphcore/_graphcore_c.h"
#include "rntree.h"
#include "cliquetree.h"

namespace HabibMcConnell2000
{
	/* Algorithm 2, p63: Lex-BFS Ordering */
	QList<Vertex*> lexBFSOrder(QList<Vertex*> component);

	/* Algorithm 3, p64: Chordality Test */
	bool isChordal(QList<Vertex*> component, RNTree* t=0);

	/* Algorithm 4, p65: Clique Tree */
	// Incorporated into the CliqueTree class constructor

	/* Algorithm 9, p76: Interval Graph Recognition (via Clique Partition Refinement) */
	QList<QSet<Vertex*> > cliqueChain(Graph const* graph);
	QList<QSet<Vertex*> > cliqueChain(QList<Vertex*> component);
}



#endif // HabibMcConnell2000_H
