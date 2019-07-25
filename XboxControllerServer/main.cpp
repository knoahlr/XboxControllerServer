#include "XboxControllerServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	XboxControllerServer w;
	w.show();
	
	return a.exec();
}
