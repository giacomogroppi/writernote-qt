#ifndef SETTING_GUI
#define SETTING_GUI

#include "../mainwindow.cpp"
#include <QFontComboBox>

class ui_final: public MainWindow{
public:
    QFontComboBox fontBox;
    void setting_final();
};

void setting_final(){
    /*
    self.style_text = QtWidgets.QToolBar("Style")
            self.style_text.setIconSize(QtCore.QSize(20, 20))
            self.addToolBar(self.style_text)

            self.fontBox = QtWidgets.QFontComboBox(self)
            self.fontBox.currentFontChanged.connect(lambda font: self.editor.setCurrentFont(font))

            self.fontSize = QtWidgets.QSpinBox(self)

            # Will display " pt" after each value
            self.fontSize.setSuffix(" pt")

            self.fontSize.valueChanged.connect(lambda size: self.editor.setFontPointSize(size))

            self.fontSize.setValue(14)
            self.style_text.addWidget(self.fontBox)
            self.style_text.addWidget(self.fontSize)
    */
}

#endif
