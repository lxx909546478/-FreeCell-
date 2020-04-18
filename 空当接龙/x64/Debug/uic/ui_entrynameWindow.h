/********************************************************************************
** Form generated from reading UI file 'EntryNameWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRYNAMEWINDOW_H
#define UI_ENTRYNAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntryNameWindow
{
public:

    void setupUi(QWidget *EntryNameWindow)
    {
        if (EntryNameWindow->objectName().isEmpty())
            EntryNameWindow->setObjectName(QString::fromUtf8("EntryNameWindow"));
        EntryNameWindow->resize(400, 300);

        retranslateUi(EntryNameWindow);

        QMetaObject::connectSlotsByName(EntryNameWindow);
    } // setupUi

    void retranslateUi(QWidget *EntryNameWindow)
    {
        EntryNameWindow->setWindowTitle(QApplication::translate("EntryNameWindow", "EntryNameWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntryNameWindow: public Ui_EntryNameWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRYNAMEWINDOW_H
