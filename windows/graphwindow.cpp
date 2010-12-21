#include "graphaction.h"

#include "graphwindow.h"

GraphWindow::GraphWindow(GraphShopWindow *parent): QDockWidget(parent)
{
	// TODO: This could also just be a number "GraphWindow1", "GraphWindow2", etc...
	setObjectName(QString::number((qulonglong)this));

	_built = false;

	setVisible(false);
	parent->addDockWidget(Qt::TopDockWidgetArea, this);

	connect(this, SIGNAL(visibilityChanged(bool)), SLOT(handleVisibilityChange(bool)));
}

void GraphWindow::build()
{
	if(!_built)
	{
		QWidget* frame = new QWidget(this);
		QVBoxLayout* layout = new QVBoxLayout();
		layout->setMargin(0);

		_toolbar = new QToolBar();

		layout->addWidget(_toolbar);

		frame->setLayout(layout);
		setWidget(frame);

		_build();

		updateWindowTitle();

		_built = true;
	}
}

void GraphWindow::setTitle(QString title)
{
	_title = title;

	updateWindowTitle();
}

void GraphWindow::updateWindowTitle()
{
	setWindowTitle(_title);
}

void GraphWindow::handleVisibilityChange(bool visible)
{
	if(visible)
	{
		build();
	}
}
