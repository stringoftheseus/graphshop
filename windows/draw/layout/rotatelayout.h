#ifndef ROTATELAYOUT_H
#define ROTATELAYOUT_H

#include "drawlayout.h"

class RotateLayout : public DrawLayout
{
    Q_OBJECT
public:
	explicit RotateLayout(double t);
	void layout();

protected:
	double _t;
};

#endif // ROTATELAYOUT_H
