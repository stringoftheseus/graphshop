#ifndef vertex_h
#define vertex_h

#include "_graphcore_c.h"

class Vertex: public QObject
{
Q_OBJECT
protected:
	Vertex(Graph* parent, QString vertexLabel = "");
	~Vertex();

	Q_DISABLE_COPY(Vertex)

public:
	Q_INVOKABLE QList<Vertex*> neighborhood();
	Q_INVOKABLE QList<Vertex*> inNeighborhood();
	Q_INVOKABLE QList<Vertex*> outNeighborhood();

	Q_INVOKABLE int index() const;
	Q_INVOKABLE QString label() const;
	Q_INVOKABLE void setLabel(QString newLabel);

	Q_INVOKABLE QList<Edge*> edges();
	Q_INVOKABLE QList<Edge*> edgesWith(Vertex* other);

	Q_INVOKABLE QList<Arc*> arcs();
	Q_INVOKABLE QList<Arc*> arcsWith(Vertex* other);

	Q_INVOKABLE QList<Arc*> inArcs();
	Q_INVOKABLE QList<Arc*> arcsFrom(Vertex* other);

	Q_INVOKABLE QList<Arc*> outArcs();
	Q_INVOKABLE QList<Arc*> arcsTo(Vertex* other);

	friend class Graph;

signals:
	void labelChanged(QString);


private:
	const Graph* graph;
	QString vertexLabel;

	QList<Edge*> E;
	QList<Arc*> I;
	QList<Arc*> O;
};

Q_DECLARE_METATYPE(Vertex*)
Q_DECLARE_METATYPE(QList<Vertex*>)

#endif
