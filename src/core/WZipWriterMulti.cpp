#include "WZipWriterMulti.h"

WZipWriterMulti::WZipWriterMulti(const char *fileZip, const char *fileInZip,
                            int thread, size_t *seek):
    _data(seek[thread - 1])
{
    int i;

    this->_size_file        = seek[thread - 1];
    this->_thread           = thread;
    this->_zip              = new WZipWriter;
    this->_writer           = new WZipWriterSingle[thread];
    this->_is_err           = false;
    this->_nameFileInZip    = fileInZip;

#ifdef DEBUGINFO
    memset(_data.get_data(), 0, seek[thread - 1]);
#endif

    this->set_zip(fileZip);
    this->set_seek(seek);
}

WZipWriterMulti::WZipWriterMulti(
        const char *fileZip,
        const char *fileInZip,
        int thread,
        size_t *seek,
        WZipWriterSingle &writer):

    _data(writer.get_data()),
    _thread(thread)
{
    int i;

    this->_writer           = new WZipWriterSingle[thread];
    this->_is_err           = false;
    this->_nameFileInZip    = fileInZip;

    this->_zip = fileZip ? new WZipWriter : NULL;
    if(fileZip){
        this->set_zip(fileZip);
    }

    this->set_seek(seek);
}

void WZipWriterMulti::set_zip(const char *fileZip)
{
    if(this->_zip->init(fileZip)){
        this->set_err();
        return;
    }
}

void WZipWriterMulti::set_seek(size_t *seek)
{
    int i;
    auto *data = static_cast<char *> (_data.get_data());

    this->_writer[0].init(data, 0, seek[0]);

    for(i = 1; i < this->_thread; i++){
        this->_writer[i].init( data, seek[i - 1], seek[i] );
    }
}

WZipWriterMulti::~WZipWriterMulti()
{
    delete      _zip;
    delete []   _writer;
}

/*
 * the save function writes the data to file, commits and then closes the file
*/
int WZipWriterMulti::commit()
{
    this->_zip->write(
                this->_data.get_data(),
                this->_size_file,
                this->_nameFileInZip
                );

    return 0;
}
