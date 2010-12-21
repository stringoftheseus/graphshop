#include "gravitylayout.h"

#include <QtGui>

#include "windows/draw/graphdraw.h"

#include "windows/draw/arcdraw.h"
#include "windows/draw/edgedraw.h"
#include "windows/draw/vertexdraw.h"

GravityLayout::GravityLayout()
{
}

void GravityLayout::layout()
{
	QMap<Vertex*, VertexDraw*> vtable;
	QMap<VertexDraw*, QPointF> ptable;

	foreach(VertexDraw* vertexdraw, vertexdraws)
	{
		vtable.insert(vertexdraw->getVertex(), vertexdraw);
	}


	QMap<Vertex*, QList<Vertex*> > ctable;
	QList<QList<Vertex*> > components = graphdraw->getGraph()->getComponents();

	foreach(QList<Vertex*> component, components)
	{
		foreach(Vertex* vertex, component)
		{
			if(vtable.contains(vertex))
			{
				ctable.insert(vertex, component);
			}
			else
			{
				component.removeAll(vertex);
			}
		}
	}


	/* Gravity Algorithm IV:
	 *
	 * If the "other" vertex is a nieghbor of this one, or in a different
	 * component, then we move based on an ideal distance away in px.
	 *
	 * If the other vertex is in the same component, but not a neighbor,
	 * then we try to get as far away from it as possible
	 */

	const int framespeed = 50;
	const double vertexspeed = 5;


	const double nideal = 50;
	const double cideal = 85;
	const double cgrav  = 100;

	foreach(VertexDraw* vdthis, vertexdraws)
	{
		Vertex* vthis = vdthis->getVertex();

		foreach(VertexDraw* vdthat, vertexdraws)
		{
			Vertex* vthat = vdthat->getVertex();

			if(vthis != vthat)
			{
				QPointF pthis = vdthis->pos();
				QPointF pthat = vdthat->pos();

				QLineF vline = QLineF(pthat, pthis);
				double llength = vline.length();

				if(vthis->neighborhood().contains(vthat))
				{
					// Try to be an "ideal" distance apart

					double ldiff = llength - nideal;

					vline.setLength(llength - ldiff*0.25);
				}
				else if(ctable[vthis] != ctable[vthat])
				{
					// Try to be an "ideal" distance apart
					double ldiff = llength - cideal;

					vline.setLength(llength - ldiff*0.05);
				}
				else
				{
					// Get as far away as possible
					vline.setLength(llength+cgrav/llength);
				}

				ptable[vdthis] += vline.p2() - vdthis->pos();
			}
		}
	}


	double diffMax = 0;
	foreach(VertexDraw* vd, vertexdraws)
	{
		diffMax = qMax(diffMax, ptable[vd].manhattanLength());
	}
	double damper = qMin(1.0, vertexspeed / diffMax);


	bool needmore = false;
	foreach(VertexDraw* vd, vertexdraws)
	{
		if(ptable[vd].manhattanLength() > 0.5)
		{
			vd->setPos(vd->pos() + ptable[vd]*damper);

			needmore = true;
		}
	}

	if(needmore)
	{
		QTimer::singleShot(framespeed, this, SLOT(layout()));
	}
}
