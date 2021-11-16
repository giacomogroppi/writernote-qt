#ifndef QFILECHOOSE_H
#define QFILECHOOSE_H

#define TYPEFILEWRITER 0
#define TYPEFILEPDF 1
#define TYPEAUDIO 2
#define TYPELOG 3

#include <QString>

class qfilechoose
{
public:
    static bool getFileForSave(QString &, uint16_t type_ = TYPEFILEWRITER);
    static bool getFileForLoad(QString &, uint16_t type_ = TYPEFILEWRITER);
};

#endif // QFILECHOOSE_H
