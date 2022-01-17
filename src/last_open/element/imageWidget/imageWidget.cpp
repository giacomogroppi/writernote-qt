#include "imageWidget.h"
#include "utils/common_script.h"

imageWidget::imageWidget(QWidget *parent, QPixmap *pixmap) : QWidget(parent)
{
    gridLayout = new QGridLayout();
    imgDisplayLabel = new QLabel("");
    scrollArea = new QScrollArea();

    if(unlikely(!pixmap)){
        this->hide();
        return;
    }

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
