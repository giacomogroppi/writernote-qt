#pragma once

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
