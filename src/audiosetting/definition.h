#ifndef DEFINITION_H
#define DEFINITION_H

#include <QString>
#include <QAudioEncoderSettings>



#ifdef __linux__
    #define POSIZIONESETTINGS "/etc/writernote/conficbinary"
    #define POSIZIONEAUDIOPERMISSION "/etc/writernote/permissionaudio"
#elif _WIN32
    #define POSIZIONESETTINGS ""

#endif


#endif // DEFINITION_H
