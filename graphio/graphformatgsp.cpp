#include "graphcore/_graphcore_h.h"
#include "windows/_panels_h.h"

#include "graphshopwindow.h"
#include "graphformatgsp.h"


GraphFormatGSP::GraphFormatGSP(GraphShopWindow* window): _window(window)
{
}

void GraphFormatGSP::save(QTextStream& dest)
{
	// 1. Start with a header section with date and such
	dest << "# GraphShop Graph" << endl;
	dest << "# " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ssap") << endl << endl;

	// 2. Add each graph to the file in turn
	foreach(Graph* graph, gsApp->graphList())
	{
		saveGraph(graph, dest);
		dest << endl;
	}

	// 3. Finish with an EOF line
	dest << '$';
}

bool GraphFormatGSP::load(QTextStream& source)
{
	// 1. Read the first line, removing the '\n' char from end
	QString line = source.readLine().trimmed();

	// 2. Read the file a line at a time
	while(source.atEnd() == false)
	{
		if(line.isEmpty() || line[0] == '#')
		{
			// 2a. Ignore comments and blank lines
			line = source.readLine().trimmed();
		}
		else if(line == "$")
		{
			// 2b. End of file
			break;
		}
		else if(line == "+Graph")
		{
			// 2c. Start a new Graph
			line = loadGraph(gsApp->addNewGraph(),  source);
		}
	}

	// 3. If nothing broke yet, assume it worked
	return true;
}


void GraphFormatGSP::saveGraph(Graph* graph, QTextStream& dest)
{
	// 1. Add the Graph Properties section
	dest << "+Graph" << endl;

	QString id = QString::number((long)graph);
	dest << "id:" << id << endl;
	_graph_id[graph] = id;

	dest << "label:" << graph->label() << endl;
	dest << "vertexCount:" << graph->vertexCount() << endl;

	// 2. Add an Arc Adjacency Matrix section if there are arcs in the graph
	if(graph->arcCount() > 0)
	{
		// 2.1. Write the section header
		dest << endl;
		dest << "=ArcAdjacencyMatrix" << endl;

		// 2.2. Write a row for each vertex in the graph
		for(int t=0; t<graph->vertexCount(); t++)
		{
			// 2.2.1. Start with an empty list of strings
			QStringList row;

			// 2.2.2. Add the arc multiplicity for each column
			for(int h=0; h<graph->vertexCount(); h++)
			{
				row.append(QString::number(graph->arcMultiplicity(t, h)));
			}

			// 2.2.3. Print a line with the multiplicities separated by spaces
			dest << row.join(" ") << endl;
		}
	}

	// 3. Add an Edge Adjacency Matrix section if there are edges in the graph
	if(graph->edgeCount() > 0)
	{
		// 3.1. Write the section header
		dest << endl;
		dest << "=EdgeAdjacencyMatrix" << endl;

		// 3.2. Add a line for each vertex in the graph
		for(int v1=0; v1<graph->vertexCount(); v1++)
		{
			// 3.2.1. Start with an empty list of strings
			QStringList row;

			// 3.2.2. Add the edge multiplicity for each column
			for(int v2=0; v2<graph->vertexCount(); v2++)
			{
				row.append(QString::number(graph->edgeMultiplicity(v1, v2)));
			}

			// 3.2.3. Print the line with the multiplicities separated by spaces
			dest << row.join(" ") << endl;
		}
	}
}


QString GraphFormatGSP::loadGraph(Graph* graph, QTextStream& source)
{
	// 1. Keep track of what we're doing later on
	int i;
	enum {GRAPH, ARCADJACENCYMATRIX, EDGEADJACENCYMATRIX} state = GRAPH;

	// 2. Read the file a line at a time
	while(source.atEnd() == false)
	{
		// 2.1. Read the next line, removing the '\n' char from end
		QString line = source.readLine().trimmed();

		// 2.2. Determine line content and act accordingly
		if(line.isEmpty() || line[0] == '#')
		{
			// 2.2a. Ignore comments and blank lines
			continue;
		}
		else if(line[0] == '+')
		{
			// 2.2b. Something else starting; we're done
			return line;
		}
		else if(line[0] == '=')
		{
			if(line == "=ArcAdjacencyMatrix")
			{
				// 2.2ca. Arc Adjacency Matrix Section
				state = ARCADJACENCYMATRIX;
				i = 0;
			}
			else if(line == "=EdgeAdjacencyMatrix")
			{
				// 2.2cb. Edge Adjacency Matrix Section
				state = EDGEADJACENCYMATRIX;
				i = 0;
			}
		}
		else if(state == GRAPH)
		{
			// 2.2d.1. Parse property line
			QString name = line.section(":", 0, 0);
			QString data = line.section(":", 1);

			// 2.2d.2. State machine based on property name
			if(name == "id")
			{
				_id_graph[data] = graph;
			}
			if(name == "vertexCount")
			{
				int count = data.toInt();
				while(graph->vertexCount() < count)
				{
					graph->addVertex();
				}
			}
			else if(name == "label")
			{
				graph->setLabel(data);
			}
		}
		else if(state == ARCADJACENCYMATRIX)
		{
			// 2.2e.1. Split arc adjacency matrix line
			QStringList arcs = line.split(' ', QString::SkipEmptyParts);

			// 2.2e.2. Make sure we have enough vertices in the graph
			while(graph->vertexCount() < arcs.count())
			{
				graph->addVertex();
			}

			// 2.2e.3. Read each arc multiplicity and set it on the graph
			for(int j=0; j<arcs.length(); j++)
			{
				graph->setArcMultiplicity(i, j, arcs[j].toInt());
			}

			i++;
		}
		else if(state == EDGEADJACENCYMATRIX)
		{
			// 2.2f.1. Split edge adjacency matrix line
			QStringList edges = line.split(' ', QString::SkipEmptyParts);

			// 2.2f.2. Make sure we have enough vertices in the graph
			while(graph->vertexCount() < edges.count())
			{
				graph->addVertex();
			}

			// 2.2f.3. Read each edge multiplicity and set it on the graph
			for(int j=0; j<edges.length(); j++)
			{
				graph->setEdgeMultiplicity(i, j, edges[j].toInt());
			}

			i++;
		}
	}

	// 3. We're at the end of the file
	return "";
}


#ifdef false
void saveAdjMatrixWindow(AdjMatrixWindow* window, QTextStream& dest);
QString loadAdjMatrixWindow(AdjMatrixWindow* window, QTextStream& source);

void saveDrawWindow(DrawWindow* window, QTextStream& dest);
QString loadDrawWindow(DrawWindow* window, QTextStream& source);

void saveIntervalGraphWindow(IntervalGraphWindow* window, QTextStream& dest);
QString loadIntervalGraphWindow(IntervalGraphWindow* window, QTextStream& source);

void saveTournamentWindow(TournamentWindow* window, QTextStream& dest);
QString loadTournamentWindow(TournamentWindow* window, QTextStream& source);
#endif




#ifdef false
bool GraphFormatGSP::load(QString fileName)
{
	QFile sourceFile(fileName);

	if(sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream sourceStream(&sourceFile);
		return load(sourceStream);
	}
	else
	{
		return false;
	}
}

bool GraphFormatGSP::load(Graph* graph, QTextStream& source)
{

}

bool GraphFormatGSP::saveGraph(Graph* graph, QString fileName)
{
	QFile destFile(fileName);
	bool success = destFile.open(QFile::WriteOnly | QIODevice::Text);

	if(success)
	{
		QTextStream destStream(&destFile);
		return save(graph, destStream);
	}
	else
	{
		return false;
	}
}

bool GraphFormatGSP::saveGraph(Graph* graph, QTextStream& dest)
{

}
#endif
