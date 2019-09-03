#include "XboxControllerServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/XboxControllerServer/Resources/atom.png"));
	XboxControllerServer w;
	w.show();
	
	return a.exec();
}
