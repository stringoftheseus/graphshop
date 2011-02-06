#include <QPrinter>

#include "graphgraphicsview.h"

GraphGraphicsView::GraphGraphicsView(QWidget *parent): QGraphicsView(parent)
{
	_exportAction = new QAction("Ex", this);
	connect(_exportAction, SIGNAL(triggered()), SLOT(doExport()));
}

QAction* GraphGraphicsView::exportAction()
{
	return _exportAction;
}

void GraphGraphicsView::doExport()
{
	QStringList filters;
	filters << "PNG (*.png)" /*<< "JPEG (*.jpg *.jpeg)" << "TIFF (*.tiff)"*/ << "PDF (*.pdf)" /*<< "EPS (*.eps)"*/;

	QFileDialog dialog(this);
	dialog.setNameFilters(filters);
	dialog.setAcceptMode(QFileDialog::AcceptSave);

	if(dialog.exec())
	{
		QString fileName = dialog.selectedFiles()[0];
		QString filter = dialog.selectedNameFilter();

		QRectF rect = scene()->itemsBoundingRect();
		rect.adjust(-10, -10, 10, 10);

		QBrush oldBackground = scene()->backgroundBrush();

		if(filter == "PDF (*.pdf)" || filter == "EPS (*.eps)" || filter == "PNG (*.png)")
		{
			scene()->setBackgroundBrush(QBrush(QColor::fromRgba(qRgba(255, 255, 255, 0))));
		}
		else
		{
			scene()->setBackgroundBrush(QBrush(QColor::fromRgba(qRgba(255, 255, 255, 255))));
		}


		if(filter == "PDF (*.pdf)" || filter == "EPS (*.eps)")
		{
			QPrinter printer;
			printer.setFullPage(true);

			printer.setOutputFileName(fileName);
			//printer.setResolution(800);

			if(filter == "PDF (*.pdf)")
			{
				printer.setOutputFormat(QPrinter::PdfFormat);
			}
			else
			{
				printer.setOutputFormat(QPrinter::PostScriptFormat);
			}


			QPainter painter;
			painter.begin(&printer);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

			render(&painter/*, scene()->itemsBoundingRect(), scene()->itemsBoundingRect().toRect()*/);
			painter.end();
		}
		else
		{
			if(filter == "PNG (*.png)")
			{
				QImage image(/*rect.*/size()/*.toSize()*/, QImage::Format_ARGB32_Premultiplied);

				QPainter painter;
				painter.begin(&image);
				painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

				render(&painter/*, QRectF(), rect.toRect()*/);
				painter.end();

				image.save(fileName);
			}
			else
			{
				QImage image(rect.size().toSize(), QImage::Format_ARGB32_Premultiplied);

				QPainter painter;
				painter.begin(&image);
				painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
				painter.fillRect(rect, Qt::white);

				render(&painter, QRectF(), rect.toRect());
				painter.end();

				image.save(fileName);
			}
		}

		scene()->setBackgroundBrush(oldBackground);
	}
}
