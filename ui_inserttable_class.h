/********************************************************************************
** Form generated from reading UI file 'inserttable_class.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSERTTABLE_CLASS_H
#define UI_INSERTTABLE_CLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_inserttable_class
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *rows;
    QSpinBox *rows_spinbox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *columns;
    QSpinBox *columns_spinbox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *spacing;
    QSpinBox *spacing_spinbox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *padding;
    QSpinBox *padding_spinbox;
    QPushButton *pushButton;

    void setupUi(QDialog *inserttable_class)
    {
        if (inserttable_class->objectName().isEmpty())
            inserttable_class->setObjectName(QString::fromUtf8("inserttable_class"));
        inserttable_class->resize(265, 297);
        inserttable_class->setMinimumSize(QSize(265, 297));
        verticalLayoutWidget = new QWidget(inserttable_class);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(9, 9, 251, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        rows = new QLabel(verticalLayoutWidget);
        rows->setObjectName(QString::fromUtf8("rows"));

        horizontalLayout->addWidget(rows);

        rows_spinbox = new QSpinBox(verticalLayoutWidget);
        rows_spinbox->setObjectName(QString::fromUtf8("rows_spinbox"));

        horizontalLayout->addWidget(rows_spinbox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        columns = new QLabel(verticalLayoutWidget);
        columns->setObjectName(QString::fromUtf8("columns"));

        horizontalLayout_2->addWidget(columns);

        columns_spinbox = new QSpinBox(verticalLayoutWidget);
        columns_spinbox->setObjectName(QString::fromUtf8("columns_spinbox"));

        horizontalLayout_2->addWidget(columns_spinbox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        spacing = new QLabel(verticalLayoutWidget);
        spacing->setObjectName(QString::fromUtf8("spacing"));

        horizontalLayout_3->addWidget(spacing);

        spacing_spinbox = new QSpinBox(verticalLayoutWidget);
        spacing_spinbox->setObjectName(QString::fromUtf8("spacing_spinbox"));

        horizontalLayout_3->addWidget(spacing_spinbox);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        padding = new QLabel(verticalLayoutWidget);
        padding->setObjectName(QString::fromUtf8("padding"));

        horizontalLayout_4->addWidget(padding);

        padding_spinbox = new QSpinBox(verticalLayoutWidget);
        padding_spinbox->setObjectName(QString::fromUtf8("padding_spinbox"));

        horizontalLayout_4->addWidget(padding_spinbox);


        verticalLayout->addLayout(horizontalLayout_4);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(inserttable_class);

        QMetaObject::connectSlotsByName(inserttable_class);
    } // setupUi

    void retranslateUi(QDialog *inserttable_class)
    {
        inserttable_class->setWindowTitle(QCoreApplication::translate("inserttable_class", "Dialog", nullptr));
        rows->setText(QCoreApplication::translate("inserttable_class", "Rows: ", nullptr));
        columns->setText(QCoreApplication::translate("inserttable_class", "Columns:", nullptr));
        spacing->setText(QCoreApplication::translate("inserttable_class", "Cell spacing: ", nullptr));
        padding->setText(QCoreApplication::translate("inserttable_class", "Cell padding:", nullptr));
        pushButton->setText(QCoreApplication::translate("inserttable_class", "Insert", nullptr));
    } // retranslateUi

};

namespace Ui {
    class inserttable_class: public Ui_inserttable_class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSERTTABLE_CLASS_H
