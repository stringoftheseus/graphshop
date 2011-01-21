#ifndef graph_h
#define graph_h

#include "_graphcore_c.h"


class Graph: public QObject
{
	Q_OBJECT

public:
	Graph(QObject *parent);
	Graph(QString graphLabel, QObject* parent);


	static QScriptValue sGraph(QScriptContext* context, QScriptEngine* engine);

	~Graph();

public:
	Q_INVOKABLE Graph* clone();
	Q_INVOKABLE virtual void build();

	Q_INVOKABLE Vertex* addVertex(QString vertexLabel = "");
	Q_INVOKABLE void removeVertex(Vertex* vertex);
	Q_INVOKABLE void removeVertices();

	Q_INVOKABLE Vertex* getVertex(int index) const;
	Q_INVOKABLE QList<Vertex*> getVertexSet() const;
	Q_INVOKABLE int vertexCount() const;


	Q_INVOKABLE Edge* addEdge(int vertex1, int vertex2, QString edgeLabel = "");
	Q_INVOKABLE Edge* addEdge(Vertex* vertex1, Vertex* vertex2, QString edgeLabel = "");
	Q_INVOKABLE void removeEdge(Edge*);
	Q_INVOKABLE void removeEdges();

	Q_INVOKABLE int edgeMultiplicity(int vertex1, int vertex2);
	Q_INVOKABLE int edgeMultiplicity(Vertex* vertex1, Vertex* vertex2);
	Q_INVOKABLE void setEdgeMultiplicity(int vertex1, int vertex2, int count);
	Q_INVOKABLE void setEdgeMultiplicity(Vertex* vertex1, Vertex* vertex2, int count);

	Q_INVOKABLE Edge* getEdge(int index) const;
	Q_INVOKABLE bool hasEdge(int vertex1, int vertex2) const;
	Q_INVOKABLE bool hasEdge(Vertex* vertex1, Vertex* vertex2) const;
	Q_INVOKABLE QList<Edge*> getEdges(int vertex1, int vertex2) const;
	Q_INVOKABLE QList<Edge*> getEdges(Vertex* vertex1, Vertex* vertex2) const;
	Q_INVOKABLE QList<Edge*> getEdgeSet() const;
	Q_INVOKABLE int edgeCount() const;


	Q_INVOKABLE Arc* addArc(int tail, int head, QString arcLabel = "");
	Q_INVOKABLE Arc* addArc(Vertex* tail, Vertex* head, QString arcLabel = "");

	// TODO: Add removeArc(int), plus removeEdge(int), and removeVertex(int)
	Q_INVOKABLE void removeArc(Arc*);
	Q_INVOKABLE void removeArcs();

	Q_INVOKABLE Arc* getArc(int index) const;
	Q_INVOKABLE void flipArc(Arc* arc);
	Q_INVOKABLE bool hasArc(int trail, int head) const;
	Q_INVOKABLE bool hasArc(Vertex* tail, Vertex* head) const;
	Q_INVOKABLE QList<Arc*> getArcs(int tail, int head) const;
	Q_INVOKABLE QList<Arc*> getArcs(Vertex* tail, Vertex* head) const;
	Q_INVOKABLE QList<Arc*> getArcSet() const;
	Q_INVOKABLE int arcCount() const;

	Q_INVOKABLE void clear();

	Q_INVOKABLE QList<QList<Vertex*> > getComponents() const;

	Q_INVOKABLE QString label() const;
	Q_INVOKABLE void setLabel(QString newLabel);

	Q_INVOKABLE bool isComplete();
	Q_INVOKABLE bool isArcComplete();
	Q_INVOKABLE bool isEdgeComplete();
	Q_INVOKABLE bool isCompleteGraph();
	Q_INVOKABLE bool isCompleteDigraph();

	Q_INVOKABLE bool isPure();
	Q_INVOKABLE bool isPureGraph();
	Q_INVOKABLE bool isPureDigraph();

	Q_INVOKABLE bool isEmpty();
	Q_INVOKABLE bool isNull();
	Q_INVOKABLE bool isTrivial();

	Q_INVOKABLE Graph* getConverseGraph() const;
	Q_INVOKABLE Graph* getDominationGraph() const;


signals:
	void arcAdded(Arc*);
	void arcFlipped(Arc*);
	void arcDeleting(Arc*);
	void arcDeleted(int);
	void arcDeleted(Vertex*, Vertex*);

	void edgeAdded(Edge*);
	void edgeDeleting(Edge*);
	void edgeDeleted(int);
	void edgeDeleted(Vertex*, Vertex*);

	void vertexAdded(Vertex*);
	void vertexDeleting(Vertex*);
	void vertexDeleted(int);

	void graphCleared();
	void labelChanged(QString);


protected:
	QList<Arc*> A;
	QList<Edge*> E;
	QList<Vertex*> V;

private:
	QString graphLabel;
};

Q_DECLARE_METATYPE(Graph*)
Q_DECLARE_METATYPE(QList<Graph*>)

#endif
