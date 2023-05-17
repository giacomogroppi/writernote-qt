#include "get_file_dir.h"
#include <QDebug>

WListFast<WString> get_file_dir::get(const WString &dir)
{
    WListFast<WString> __l;
    QDirIterator iterator(dir, QDir::AllEntries | QDir::Hidden | QDir::System );
    while (iterator.hasNext()) {
        __l.append(iterator.next());
    }

    return __l;
}
