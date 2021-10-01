#include "imageWidget.h"

imageWidget::imageWidget(QWidget *parent, QPixmap *pixmap) : QWidget(parent)
{
    gridLayout = new QGridLayout();
    imgDisplayLabel = new QLabel("");
    scrollArea = new QScrollArea();

    imgDisplayLabel->setPixmap(*pixmap);
    imgDisplayLabel->adjustSize();
    scrollArea->setWidget(imgDisplayLabel);
    scrollArea->setMinimumSize(256,256);
    scrollArea->setMaximumSize(512,512);
    gridLayout->addWidget(scrollArea,0,0);
    setLayout(gridLayout);
}

imageWidget::~imageWidget()
{
    delete imgDisplayLabel;
    delete scrollArea;
    delete gridLayout;
}
