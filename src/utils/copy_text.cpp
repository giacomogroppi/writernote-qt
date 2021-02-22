#include "copy_text.h"

#include <QClipboard>
#include <QApplication>

void copy_text(QString text)
{
    QClipboard *clip = QApplication::clipboard();
    clip->setText(text);
}
