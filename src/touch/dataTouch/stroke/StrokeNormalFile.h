#pragma once

#include "StrokeNormal.h"

class StrokeNormalFileLoad {
private:
    StrokeNormal &_data;
public:
    StrokeNormalFileLoad();
};

class StrokeNormalFileSave {
private:
    const StrokeNormal &_data;
public:
    StrokeNormalFileSave(const StrokeNormal &_stroke);
    size_t getSizeInFile() const;
};
