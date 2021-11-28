#include "aggiornotastiriascolto.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include "../utils/common_error_definition.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../dataread/xmlstruct.h"
#include "audioplay.h"

#define ERROR_AUDIO "We had an internal problem with audio, please \nclose the application and open it again"

void aggiornotestiriascolto(MainWindow *parent){
    Document *doc = parent->m_canvas->data;

    if(doc->se_registato != Document::not_record){
        if(doc->se_registato == Document::record_file){
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
            if(QFile::exists(doc->audio_position_path))
                return dialog_critic("Audio " + doc->audio_position_path + " didn't exist");
#else
            user_message("This audio cannot be played back because it was recorded with a desktop computer.");
            return;
#endif
        }

        parent->contrUi();

        /* abilita il bottone della mano e lo segna unchecked, in caso sia cliccato per qualche motivo*/
        parent->ui->actionListen_current_audio->setEnabled(true);

        if(doc->se_registato == Document::record_zip){
            /* if it's not laoded */
            if(doc->audio_data.isEmpty())
                if(load_audio(doc->audio_data, parent->m_path) != OK)
                    return dialog_critic("We had a problem loading the audio");

            if(parent->m_buffer->isOpen()){
                /* we have to close the buffer otherwise we cannot overwrite the data */
                parent->m_buffer->close();
            }

            parent->m_buffer->setData(doc->audio_data);

            if(!parent->m_buffer->open(QIODevice::ReadOnly))
                return dialog_critic(ERROR_AUDIO);

            parent->m_audioplayer->setMedia(parent->m_buffer);

        }
        else{
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
            parent->m_audioplayer->setMedia(doc->audio_position_path);
#else
            user_message("The copybook you are opening has an audio external to the writernote file, it is not possible to load the file because you are on android");
#endif
        }

        parent->ui->actionDelete_audio->setEnabled(true);

        parent->ui->stop_rec->setEnabled(false);
        parent->ui->pause_rec->setEnabled(false);
    }
    else{
        parent->contrUi();
    }

}
