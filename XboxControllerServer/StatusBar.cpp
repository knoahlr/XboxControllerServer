#include "StatusBar.h"



DefaultStatusBar::DefaultStatusBar()
{
	statusFont = new QFont();
	statusFont->setBold(true);
	statusFont->setFamily(QString("Comic Sans MS"));
	setFont(*statusFont);

}