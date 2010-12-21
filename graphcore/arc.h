#ifndef arc_h
#define arc_h

#include "_graphcore_c.h"

class Arc: public QObject
{
Q_OBJECT

protected:
	Arc(Graph* parent, Vertex* tail, Vertex* head, QString arcLabel = "");
	~Arc();

	Q_DISABLE_COPY(Arc)

public:
	static QScriptValue sArc(QScriptContext* context, QScriptEngine* engine);


	Q_INVOKABLE Vertex* head() const;
	Q_INVOKABLE Vertex* tail() const;

	Q_INVOKABLE int index() const;
	Q_INVOKABLE QString label() const;
	Q_INVOKABLE void setLabel(QString newLabel);

	friend class Graph;

signals:
	void labelChanged(QString);

private:
	QString arcLabel;

	Vertex* vtail;
	Vertex* vhead;
};

Q_DECLARE_METATYPE(Arc*)
Q_DECLARE_METATYPE(QList<Arc*>)

#endif
