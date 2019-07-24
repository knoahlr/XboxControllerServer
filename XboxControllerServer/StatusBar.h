#pragma once
#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QHBoxLayout>
#include <string.h>


class DefaultStatusBar : public QStatusBar

{

public:
	DefaultStatusBar();
	QFont *statusFont;

};