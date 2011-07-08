/********************************************************************************
** Form generated from reading UI file 'quadconsole.ui'
**
** Created: Fri Jul 8 13:55:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUADCONSOLE_H
#define UI_QUADCONSOLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_quadconsole
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *quadconsole)
    {
        if (quadconsole->objectName().isEmpty())
            quadconsole->setObjectName(QString::fromUtf8("quadconsole"));
        quadconsole->resize(400, 300);
        menuBar = new QMenuBar(quadconsole);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        quadconsole->setMenuBar(menuBar);
        mainToolBar = new QToolBar(quadconsole);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        quadconsole->addToolBar(mainToolBar);
        centralWidget = new QWidget(quadconsole);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        quadconsole->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(quadconsole);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        quadconsole->setStatusBar(statusBar);

        retranslateUi(quadconsole);

        QMetaObject::connectSlotsByName(quadconsole);
    } // setupUi

    void retranslateUi(QMainWindow *quadconsole)
    {
        quadconsole->setWindowTitle(QApplication::translate("quadconsole", "quadconsole", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class quadconsole: public Ui_quadconsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUADCONSOLE_H
