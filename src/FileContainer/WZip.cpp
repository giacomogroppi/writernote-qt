#include "WZip.h"
#include "testing/memtest.h"
#include "FileContainer/WZipCommon.h"

WZip::WZip(const WPath &path, bool &ok)
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
    this->_data_private._data = nullptr;
    this->_data_private._status.set_data_not_available();
}

WZip::~WZip()
{
    WFree(this->_data_private._data);

    if (_data_private._have_to_close and _data_private._zip) {
        _data_private._zip->close();
    }

    DO_IF_DEBUG(this->_data_private._zip = nullptr;)
}

bool WZip::openZip(const WPath &pathZip)
{
    W_ASSERT(!pathZip.operator std::filesystem::path().empty());
    W_ASSERT(this->_data_private._zip == nullptr);
    W_ASSERT(this->_data_private._data == nullptr);

    this->_data_private._zip = new FileContainer(pathZip);

    if(_data_private._zip){
        this->_data_private._status.set_zip_open();
    }

    return _data_private._zip != nullptr;
}

// return true on success
bool WZip::openFileInZip(const WByteArray &nameFile)
{
    W_ASSERT(_data_private._zip);
    W_ASSERT(this->_data_private._status.is_zip_open());
    W_ASSERT(!this->_data_private._status.is_data_available());


    this->_data_private._len_file = WZip::get_size_file(_data_private._zip, nameFile.constData());
    this->_data_private._data = WMalloc(_data_private._len_file);

    auto file = _data_private._zip->getFileReader(nameFile);

    if(!file.isOk())
        return false;

    const auto res = file.read(_data_private._data, _data_private._len_file);

    if(res < 0){
        _data_private._zip->close();
        WFree(_data_private._data);
        _data_private._data = nullptr;
        _data_private._status.set_data_not_available();
        return false;
    }

    _data_private._zip->close();
    _data_private._status.set_data_available();
    return true;
}
