#include "xmlstruct.h"
#include "core/String/WString.h"
#include "utils/WCommonScript.h"
#include "file/File.h"

xmlstruct::xmlstruct(const WPath *path_U, Document *currenttitle_U)
{
    setData(path_U, currenttitle_U);
}

void xmlstruct::setData(const WPath *path_U, Document *currenttitle_U)
{
    this->_path = path_U;
    this->_doc = currenttitle_U;
}

size_t xmlstruct::get_size_file(const WPath &path)
{
    size_t size;
    WFile file(path);

    if (!file.open(WFile::WFileReadOnly))
        return 0;

    size = file.size();

    file.close();

    return size;
}

xmlstruct::xmlstruct(const WPath &path_U, Document &currenttitle_U)
{
    setData(&path_U, &currenttitle_U);
}
