#pragma once

#include "zip.h"
#include "utils/WCommonScript.h"
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

struct WZipPrivate{
    void            *_data      = NULL;
    zip_t           *_zip       = NULL;
    size_t          _len_file   = 0;
    QString         _path;
    PrivateStatus   _status;
    bool            _have_to_close;
};

class WZip
{
private:
    struct WZipPrivate _data_private;

    bool openZip(const QByteArray &path);
    zip_file_t *open_file_in_zip(const QByteArray &path);

public:
    WZip(const QByteArray &path, bool &ok);
    WZip(WZip &zip);
    ~WZip();

    void close_zip();
    void dealloc_file();
    zip_t *get_zip() const;

    bool openFileInZip(const QByteArray &path);

    [[nodiscard]] size_t length() const;

    static size_t get_size_file(zip_t *zip, const char *name);

    const char *get_data() const;

    bool is_data_available() const;

    WZip &operator=(WZip &&other) = delete;
    WZip &operator=(WZip &other)  = delete;
    WZip &operator=(WZip other)   = delete;
};

inline bool WZip::is_data_available() const
{
#ifdef DEBUGINFO
    if(_data_private._status.is_data_available())
        W_ASSERT(this->_data_private._data);
    else
        W_ASSERT(!this->_data_private._data);
#endif
    return _data_private._status.is_data_available();
}

force_inline size_t WZip::length() const
{
    W_ASSERT(_data_private._len_file > 0);
    W_ASSERT(this->_data_private._status.is_data_available());
    return this->_data_private._len_file;
}

force_inline const char *WZip::get_data() const
{
    W_ASSERT(this->_data_private._data);
    return (const char *)this->_data_private._data;
}

force_inline zip_t* WZip::get_zip() const
{
    W_ASSERT(this->_data_private._zip);
    return this->_data_private._zip;
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
