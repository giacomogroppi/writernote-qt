#include "retry_save_audio.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/path/get_path.h"
#include <QFile>

retry_save_audio::retry_save_audio(currenttitle_class *m_data, bool *exist_)
{
    this->m_current = m_data;
    this->m_exist = exist_;
}

retry_save_audio::~retry_save_audio()
{

}

bool retry_save_audio::exist(){
    QString path = get_path_no_controll();
    return QFile::exists(path + this->m_current->nome_copybook + POS_TEMP_AUDIO);
}

void retry_save_audio::retry(){
    *this->m_exist = this->exist();

    emit resultRetry(*this->m_exist);
}

void retry_save_audio::close(){
    if(*this->m_exist)
        return;

    /* reset audio */
    this->m_current->se_registato = currenttitle_class::not_record;
    this->m_current->testinohtml.clear();
    this->m_current->posizione_iniz.clear();

}
