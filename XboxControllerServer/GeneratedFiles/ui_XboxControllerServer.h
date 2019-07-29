/********************************************************************************
** Form generated from reading UI file 'XboxControllerServer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XBOXCONTROLLERSERVER_H
#define UI_XBOXCONTROLLERSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XboxControllerServerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XboxControllerServerClass)
    {
        if (XboxControllerServerClass->objectName().isEmpty())
            XboxControllerServerClass->setObjectName(QStringLiteral("XboxControllerServerClass"));
        XboxControllerServerClass->resize(600, 400);
        menuBar = new QMenuBar(XboxControllerServerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        XboxControllerServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XboxControllerServerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        XboxControllerServerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(XboxControllerServerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        XboxControllerServerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(XboxControllerServerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        XboxControllerServerClass->setStatusBar(statusBar);

        retranslateUi(XboxControllerServerClass);

        QMetaObject::connectSlotsByName(XboxControllerServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *XboxControllerServerClass)
    {
        XboxControllerServerClass->setWindowTitle(QApplication::translate("XboxControllerServerClass", "XboxControllerServer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XboxControllerServerClass: public Ui_XboxControllerServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XBOXCONTROLLERSERVER_H
