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
    imageWidget(QWidget *parent = 0, QPixmap *pixmap = nullptr);
    ~imageWidget();
private:
    QGridLayout* gridLayout;
    QLabel* imgDisplayLabel;
    QScrollArea* scrollArea;
};

#endif // IMAGEWIDGET_H
