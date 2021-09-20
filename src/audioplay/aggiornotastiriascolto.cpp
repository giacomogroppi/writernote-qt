#include "aggiornotastiriascolto.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>

#include "../utils/dialog_critic/dialog_critic.h"
#include "../setting_ui.h"
#include "../dataread/xmlstruct.h"
#define ERROR_AUDIO "We had an internal problem with audio, please \nclose the application and open it again"


void aggiornotestiriascolto(MainWindow *parent){
    if(parent->m_currenttitle->se_registato != Document::not_record){
        if(parent->m_currenttitle->se_registato == Document::record_file){
#ifndef ANDROID
            if(QFile::exists(parent->m_currenttitle->audio_position_path))
                return dialog_critic("Audio " + parent->m_currenttitle->audio_position_path + " didn't exist");
#else
            user_message("It's not possibile to play this audio");
#endif
        }

        parent->contrUi();

        /* abilita il bottone della mano e lo segna unchecked, in caso sia cliccato per qualche motivo*/
        parent->ui->actionListen_current_audio->setEnabled(true);

        if(parent->m_currenttitle->se_registato == Document::record_zip){
            /* if it's not laoded */
            if(parent->m_currenttitle->audio_data.isEmpty())
                if(load_audio(parent->m_currenttitle->audio_data, parent->m_path) != OK)
                    return dialog_critic("We had a problem loading the audio");

            if(parent->m_buffer->isOpen()){
                /* we have to close the buffer otherwise we cannot overwrite the data */
                parent->m_buffer->close();
            }

            parent->m_buffer->setData(parent->m_currenttitle->audio_data);

            if(!parent->m_buffer->open(QIODevice::ReadOnly))
                return dialog_critic(ERROR_AUDIO);

            parent->player->setMedia(QMediaContent(), parent->m_buffer);

        }
        else{
#ifndef ANDROID
            parent->player->setMedia(QUrl::fromLocalFile(parent->m_currenttitle->audio_position_path));
#else
            user_message("The copybook you are opening has an audio external to the writernote file, it is not possible to load the file because you are on android");
#endif
        }

        parent->ui->actionDelete_audio->setEnabled(true);

        parent->ui->stop_rec->setEnabled(false);
        parent->ui->pause_rec->setEnabled(false);
    }
    else{
        /* abilita i tasti per la registrazione */
        parent->contrUi();

        parent->ui->actionDelete_audio->setEnabled(false);

        //parent->ui->actionListen_current_audio->setChecked(false);
        parent->ui->actionListen_current_audio->setEnabled(false);
    }

    if(parent->m_currenttitle->se_tradotto){

    }
}
