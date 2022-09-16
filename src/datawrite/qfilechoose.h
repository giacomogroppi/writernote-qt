#pragma once


#include "utils/common_script.h"
#include <QString>

#define TYPEFILEWRITER BIT(1)
#define TYPEFILEPDF BIT(2)
#define TYPEAUDIO BIT(3)
#define TYPELOG BIT(4)
#define TYPEALL BIT(5)

namespace qfilechoose
{
    bool getFileForSave(QString &path, uint16_t type_ = TYPEFILEWRITER);
    bool getFileForLoad(QString &path, uint16_t type_ = TYPEFILEWRITER);
};
