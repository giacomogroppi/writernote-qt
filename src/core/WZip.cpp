#include "WZip.h"
#include "testing/memtest.h"
#include "core/WZipCommon.h"

WZip::WZip(const WByteArray &path, bool &ok)
{
    ok = this->openZip(path);
    if(ok){
        this->_data_private._status.set_zip_open();
        this->_data_private._status.set_data_not_available();
    }

    this->_data_private = {
        ._data = nullptr,
        ._zip = _data_private._zip,
        ._len_file = 0,
        ._path = path,
        ._status = _data_private._status,
        ._have_to_close = true
    };
}

WZip::WZip(WZip &zip)
{
    W_ASSERT(zip._data_private._zip != nullptr);

    this->_data_private = {
        ._data = nullptr,
        ._zip = zip._data_private._zip,
        ._len_file = 0,
        ._path = zip._data_private._path,
        ._status = zip._data_private._status,
        ._have_to_close = false
    };
}

void WZip::dealloc_file()
{
    W_ASSERT(this->_data_private._data);
    W_ASSERT(this->_data_private._status.is_data_available());
    WFree(this->_data_private._data);
    this->_data_private._data = NULL;
    this->_data_private._status.set_data_not_available();
}

WZip::~WZip()
{
    WFree(this->_data_private._data);

    if(_data_private._have_to_close and _data_private._zip){
        zip_close(this->_data_private._zip);
    }

    DO_IF_DEBUG(this->_data_private._zip = nullptr;)
}

bool WZip::openZip(const WByteArray &pathZip)
{
    W_ASSERT(!pathZip.isEmpty());
    W_ASSERT(this->_data_private._zip == nullptr);
    W_ASSERT(this->_data_private._data == nullptr);

    this->_data_private._zip = WZipCommon::openZip(pathZip.constData(), W_ZIP_READ);

    if(_data_private._zip){
        this->_data_private._status.set_zip_open();
    }

    return _data_private._zip != nullptr;
}

zip_file_t *WZip::open_file_in_zip(const WByteArray &nameFile)
{
    zip_file_t *file;

    file = zip_fopen(_data_private._zip, nameFile.constData(), 0);

    return file;
}

// return true on success
bool WZip::openFileInZip(const WByteArray &nameFile)
{
    W_ASSERT(_data_private._zip);
    W_ASSERT(this->_data_private._status.is_zip_open());
    W_ASSERT(!this->_data_private._status.is_data_available());

    zip_file_t *file;

    this->_data_private._len_file = WZip::get_size_file(_data_private._zip, nameFile.constData());
    this->_data_private._data = WMalloc(_data_private._len_file);

    file = open_file_in_zip(nameFile);

    if(!file)
        return false;

    const int64_t res = zip_fread(file, _data_private._data, _data_private._len_file);

    if(res != static_cast<int64_t>(_data_private._len_file)){
        zip_fclose(file);
        WFree(_data_private._data);
        _data_private._data = nullptr;
        _data_private._status.set_data_not_available();
        return false;
    }

    zip_fclose(file);
    _data_private._status.set_data_available();
    return true;
}
