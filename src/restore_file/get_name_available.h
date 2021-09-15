#ifndef GET_NAME_AVAILABLE_H
#define GET_NAME_AVAILABLE_H

#include <QString>
class Document;

class get_name_available
{
public:
    static QString get(const QString &path, bool &ok, Document *doc);
};

#endif // GET_NAME_AVAILABLE_H
