#include "retry_save_audio.h"
#include "../currenttitle/document.h"
#include "../utils/path/get_path.h"
#include <QFile>

retry_save_audio::retry_save_audio(Document *m_data, bool *exist_)
{
    this->m_current = m_data;
    this->m_exist = exist_;
}

retry_save_audio::~retry_save_audio()
{

}

bool retry_save_audio::exist(){
    QString path = get_path_no_controll();
    return QFile::exists(path + POS_TEMP_AUDIO);
}

void retry_save_audio::retry(){
    *this->m_exist = this->exist();

    emit resultRetry(*this->m_exist);
}

void retry_save_audio::close(){
    if(*this->m_exist)
        return;

    /* reset audio */
    this->m_current->se_registato = Document::not_record;
    this->m_current->cleanAudio();

}
