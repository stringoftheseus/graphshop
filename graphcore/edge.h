#ifndef edge_h
#define edge_h

#include "_graphcore_c.h"
//#include "graph.h"

class Edge: public QObject
{
Q_OBJECT
protected:
	Edge(Graph* parent, Vertex* vertex1, Vertex* vertex2, QString edgeLabel = "");
	~Edge();

	Q_DISABLE_COPY(Edge)

public:
	static QScriptValue sEdge(QScriptContext* context, QScriptEngine* engine);

	Q_INVOKABLE Vertex* vertex1() const;
	Q_INVOKABLE Vertex* vertex2() const;

	Q_INVOKABLE int index() const;
	Q_INVOKABLE QString label() const;
	Q_INVOKABLE void setLabel(QString newLabel);

	friend class Graph;

signals:
	void labelChanged(QString);

private:
	QString edgeLabel;

	Vertex* v1;
	Vertex* v2;
};

Q_DECLARE_METATYPE(Edge*)
Q_DECLARE_METATYPE(QList<Edge*>)

#endif
