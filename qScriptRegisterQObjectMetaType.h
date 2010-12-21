#ifndef QSCRIPTREGISTERQOBJECTMETATYPE_H
#define QSCRIPTREGISTERQOBJECTMETATYPE_H

/* From Kent Hansen
 * via http://lists.trolltech.com/qt-interest/2007-10/thread00687-0.html
 */



#include <QtScript>

template <typename Tp>
QScriptValue qScriptValueFromQObject(QScriptEngine *engine, Tp const
&qobject)
{
    return engine->newQObject(qobject);
}

template <typename Tp>
void qScriptValueToQObject(const QScriptValue &value, Tp &qobject)
{   
    qobject = qobject_cast<Tp>(value.toQObject());
}

template <typename Tp>
int qScriptRegisterQObjectMetaType(
    QScriptEngine *engine,
    const QScriptValue &prototype = QScriptValue()
#ifndef qdoc
    , Tp * /* dummy */ = 0
#endif
    )
{
    return qScriptRegisterMetaType<Tp>(engine, qScriptValueFromQObject,
                                       qScriptValueToQObject, prototype);
}

#endif // QSCRIPTREGISTERQOBJECTMETATYPE_H
