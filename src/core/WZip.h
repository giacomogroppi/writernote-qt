#pragma once

#include "zip.h"
#include "utils/common_script.h"
#include <QString>
#include <QObject>

class PrivateStatus{
private:

#define ZIP_OPEN BIT(3)
#define D_AVAILABLE BIT(2)

    int _status = ~(ZIP_OPEN | D_AVAILABLE);
public:
    PrivateStatus() = default;
    ~PrivateStatus() = default;

    force_inline void set_zip_open()
    {
        _status |= ZIP_OPEN;
    }

    force_inline void set_data_not_available()
    {
        _status &= ~D_AVAILABLE;
    }

    force_inline void set_data_available()
    {
        _status |= D_AVAILABLE;
    }


    force_inline bool is_data_available() const
    {
        return _status & D_AVAILABLE;
    }

    force_inline bool is_zip_open() const
    {
        return _status & ZIP_OPEN;
    }

};

class WZip
{
private:
    void *_data = NULL;

    zip_t           *_zip       = NULL;
    size_t          _len_file   = 0;

    QString path;
    PrivateStatus _status;

    bool openZip(const QByteArray &path);

public:
    WZip(const QByteArray &path, bool &ok);
    ~WZip();

    void close_zip();
    void dealloc_file();
    zip_t *get_zip();

#ifdef ALL_VERSION
    zip_file_t *open_file_in_zip(const QByteArray &path);
#endif

    bool openFileInZip(const QByteArray &path);

    [[nodiscard]] size_t length() const;

    static size_t get_size_file(zip_t *zip, const char *name);

    const char *get_data();

    WZip &operator=(WZip &&other) = delete;
    WZip &operator=(WZip &other)  = delete;
    WZip &operator=(WZip other)   = delete;
};

force_inline size_t WZip::length() const
{
    W_ASSERT(_len_file > 0);
    W_ASSERT(this->_status.is_data_available());
    return this->_len_file;
}

force_inline const char *WZip::get_data()
{
    W_ASSERT(this->_data);
    return (const char *)this->_data;
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
