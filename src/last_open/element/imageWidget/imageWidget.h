#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include <QDialog>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPixmap>


class imageWidget : public QWidget
{
    Q_OBJECT

public:
    imageWidget(QWidget *parent, QPixmap *pixmap);
    ~imageWidget();
private:
    QGridLayout* gridLayout;
    QLabel* imgDisplayLabel;
    QScrollArea* scrollArea;
};

#endif // IMAGEWIDGET_H
