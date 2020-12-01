#ifndef SETTING_UI
#define SETTING_UI

#include "setting_ui.h"

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QFontComboBox>
#include <QToolBar>
#include <QDebug>

/*void QFontComboBox::currentFontChanged(const QFont &font){
    qDebug() << "funzione richiamata";
}*/

/*Funzione che gestisce l'aggiunta della barra per selezionare il font*/
void setting_ui(MainWindow *parent){
    QToolBar* toolbartext = new QToolBar;
    parent->addToolBar(Qt::TopToolBarArea, toolbartext);

    QFontComboBox* myComboBox = new QFontComboBox(parent);
    toolbartext->addWidget(myComboBox);

    toolbartext->setIconSize(QSize(20, 20));

    //myComboBox->connect(myComboBox, SIGNAL(currentFontChanged(const QFont &f)), parent, SLOT(changeTextFont()));

}
//
#endif // SETTING_UI

/*
self.style_text = QtWidgets.QToolBar("Style")
self.style_text.setIconSize(QtCore.QSize(20, 20))
self.addToolBar(self.style_text)
self.fontBox = QtWidgets.QFontComboBox(self)
self.fontBox.currentFontChanged.connect(lambda font: self.editor.setCurrentFont(font))
self.fontSize = QtWidgets.QSpinBox(self)

self.fontSize.valueChanged.connect(lambda size: self.editor.setFontPointSize(size))

self.fontSize.setValue(14)
self.style_text.addWidget(self.fontBox)
self.style_text.addWidget(self.fontSize)
*/
