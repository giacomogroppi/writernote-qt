#pragma once

#include "StrokeNormal.h"
#include "FileContainer/WZipReaderSingle.h"
#include "FileContainer/WZipWriterSingle.h"

class StrokeNormalFileLoad {
private:
    StrokeNormal &_data;
public:
    StrokeNormalFileLoad();
};
