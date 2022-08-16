#pragma once

#include "zip.h"
#include "utils/common_script.h"
#include <QString>
#include <QObject>

class PrivateStatus{
private:
#define FILE_OPEN BIT(1)
#define ZIP_OPEN BIT(3)
#define D_AVAILABLE BIT(2)

    int _status = ~(ZIP_OPEN | FILE_OPEN | D_AVAILABLE);
public:
    PrivateStatus() = default;
    ~PrivateStatus() = default;

    force_inline void set_zip_open()
    {
        _status |= ZIP_OPEN;
    }

    force_inline void set_file_open()
    {
        _status |= FILE_OPEN;
    }

    force_inline void set_data_available()
    {
        _status |= D_AVAILABLE;
    }


    force_inline bool isDataAvailable() const
    {
        return _status & D_AVAILABLE;
    }

    force_inline bool is_zip_open() const
    {
        return _status & ZIP_OPEN;
    }

    force_inline bool is_file_open() const
    {
        W_ASSERT(this->is_zip_open());
        return _status & FILE_OPEN;
    }

};

class WZip
{
private:
    void *_data = NULL;

    zip_t           *_zip = NULL;
    zip_file_t      *_file = NULL;

    QString path;
    PrivateStatus _status;

    bool openZip(const QByteArray &path);

public:
    WZip(const QByteArray &path, bool &ok);
    ~WZip();

    void close_zip();
    zip_t *get_zip();
    zip_file_t *get_file();
    bool openFileInZip(const QByteArray &path);
    bool alloc_and_close(const char *fileName);

    static size_t get_size_file(zip_t *zip, const char *name);

    static zip_t *openZipWrite(const QByteArray &path);

    WZip &operator=(WZip &&other) = delete;
    WZip &operator=(WZip &other)  = delete;
    WZip &operator=(WZip other)   = delete;
};

force_inline zip_file_t *WZip::get_file()
{
    W_ASSERT(this->_file);
    return this->_file;
}

force_inline zip_t* WZip::get_zip()
{
    W_ASSERT(this->_zip);
    return this->_zip;
}

force_inline size_t WZip::get_size_file(zip_t *zip, const char *name)
{
    struct zip_stat st;
    zip_stat_init(&st);

    /*
     * Upon successful completion 0 is returned. Otherwise,
     * -1 is returned and the error information in archive
     * is set to indicate the error
    */
    if(zip_stat(zip, name, ZIP_STAT_SIZE, &st) < 0)
        return 0;

    return st.size;
}
