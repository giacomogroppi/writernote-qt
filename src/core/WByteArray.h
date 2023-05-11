#ifndef WRITERNOTE_WBYTEARRAY_H
#define WRITERNOTE_WBYTEARRAY_H

#include <iostream>

class WByteArray {
public:
    WByteArray ();
    WByteArray(const char *data, size_t size = -1);

    std::string toStdString() const;
    const char *constData() const;
    size_t size() const;
};


#endif //WRITERNOTE_WBYTEARRAY_H
