#ifndef GRAPHFORMATGSG_H
#define GRAPHFORMATGSG_H

#include "graphcore/_graphcore_c.h"

class GraphFormatGSG
{
public:
    GraphFormatGSG();

	static bool loadFile(Graph* graph, QString fileName);
	static bool saveFile(Graph* graph, QString fileName);

	static bool load(Graph* graph, QTextStream& source);
	static bool save(Graph* graph, QTextStream& dest);
};

#endif // GRAPHFORMATGSG_H
