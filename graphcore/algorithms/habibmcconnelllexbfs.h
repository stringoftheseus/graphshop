#ifndef HABIBMCCONNELLLEXBFS_H
#define HABIBMCCONNELLLEXBFS_H

#include "graphcore/_graphcore_c.h"

class HabibMcConnellLexBFS
{
public:
	HabibMcConnellLexBFS(Graph const*);

	/* Algorithm 2, p. 63: Lex-BFS Ordering */


private:
	// The source graph
	Graph const* _graph;

	// The vertex ordering obtained from Lex-BFS
	QList<Vertex*> _pi;

	//

};

#endif // HABIBMCCONNELLLEXBFS_H
