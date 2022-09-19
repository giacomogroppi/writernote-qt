#include "WFile.h"
#include "utils/common_script.h"
#include <QFile>

int WFile::readFile(QByteArray &to, const char *pos)
{
    QFile file;
    if(!file.open(QFile::ReadOnly))
        return -1;
    W_ASSERT(to.isEmpty());
    to = file.readAll();

    const auto size = file.size();

    file.close();

    if(unlikely(size != to.size()))
        return -1;

    return 0;
}
