#ifndef graphiotext_h
#define graphiotext_h

#include <iostream>
#include <string>
#include <QtCore>

#include "../graphcore/graph.h"

class GraphIOText
{
public:
	enum Mode {ADJ_MATRIX, ADJ_LIST};

	static void fromString(Graph&, const char*);
	static void fromString(Graph&, std::string const&);
	static void fromStream(Graph&, std::istream*);
	static void fromFile(Graph&, const char*);

	static std::string toString(Graph const&, Mode);
	static void toStream(Graph const&, std::ostream*, Mode);
	static void toFile(Graph const&, const char*, Mode);

private:
	static std::string getLine(std::istream* in);

	static void parseGraphAdjMatrix(Graph&, std::istream*);
	static void parseDigraphAdjMatrix(Graph&, std::istream*);
	static void parseAdjList(Graph&, std::istream*);

	static void buildGraphAdjMatrix(Graph const&, std::ostream*);
	static void buildDigraphAdjMatrix(Graph const&, std::ostream*);
	static void buildAdjList(Graph const&, std::ostream*);
};

#endif
