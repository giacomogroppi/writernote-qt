#include "imageWidget.h"
#include "utils/common_script.h"
#include "testing/memtest.h"

imageWidget::imageWidget(QWidget *parent, QPixmap *pixmap) : QWidget(parent)
{
    WNew(gridLayout, QGridLayout, ());
    WNew(imgDisplayLabel, QLabel, (""));
    WNew(scrollArea, QScrollArea, ());

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
    WDelete(imgDisplayLabel);
    WDelete(scrollArea);
    WDelete(gridLayout);

    imgDisplayLabel = NULL;
    scrollArea = NULL;
    gridLayout = NULL;
}
