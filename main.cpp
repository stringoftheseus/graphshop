#include <QtGui/QApplication>

#include "graphshopapp.h"
#include "graphshopwindow.h"

int main(int argc, char *argv[])
{
	GraphShopApp a(argc, argv);

	GraphShopWindow window;
	window.show();

	return a.exec();
}
