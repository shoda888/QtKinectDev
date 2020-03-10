/********************************************************************************
** Form generated from reading UI file 'ImageRenderWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGERENDERWIDGET_H
#define UI_IMAGERENDERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageRenderWidget
{
public:

    void setupUi(QWidget *ImageRenderWidget)
    {
        if (ImageRenderWidget->objectName().isEmpty())
            ImageRenderWidget->setObjectName(QStringLiteral("ImageRenderWidget"));
        ImageRenderWidget->resize(400, 300);

        retranslateUi(ImageRenderWidget);

        QMetaObject::connectSlotsByName(ImageRenderWidget);
    } // setupUi

    void retranslateUi(QWidget *ImageRenderWidget)
    {
        ImageRenderWidget->setWindowTitle(QApplication::translate("ImageRenderWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageRenderWidget: public Ui_ImageRenderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGERENDERWIDGET_H
