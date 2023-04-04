#pragma once

#include <QPixmap>

class WPixmap : public QPixmap
{
public:
    WPixmap(int page, bool consideringResolution);
};

