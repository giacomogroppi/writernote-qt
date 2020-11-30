#include "../mainwindow.h"
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include "../dialog_critic.h"

void MainWindow::on_insertimagebotton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Insert image", "", "Images (*.png *.xpm *.jpg *.bmp *.gif)");

    if(filename == "")
        return;

    QImage immagine = QImage(filename);
    if(immagine.isNull())
        dialog_critic("Could not load image file!");



    else{
        auto cursor = this->ui->textEdit->textCursor();
        cursor.insertImage(immagine, filename);
    }

}

/*
        filename = QtWidgets.QFileDialog.getOpenFileName(self, 'Insert image',".","Images (*.png *.xpm *.jpg *.bmp *.gif)")[0]

        if filename:

            # Create image object
            image = QtGui.QImage(filename)

            # Error if unloadable
            if image.isNull():

                popup = QtWidgets.QMessageBox(QtWidgets.QMessageBox.Critical,
                                          "Image load error",
                                          "Could not load image file!",
                                          QtWidgets.QMessageBox.Ok,
                                          self)
                popup.show()

            else:

                cursor = self.editor.textCursor()

                cursor.insertImage(image,filename)

*/
