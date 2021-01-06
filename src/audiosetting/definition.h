#ifndef DEFINITION_H
#define DEFINITION_H

#include <QString>
#include <QAudioEncoderSettings>



#ifdef __linux__
#define POSIZIONESETTINGS "/etc/writernote/conficbinary"
#define POSIZIONEAUDIOPERMISSION "/etc/writernote/permissionaudio"
#endif

#if defined(WIN32) || defined(WIN64)
#define POSIZIONESETTINGS "testing"
#define POSIZIONEAUDIOPERMISSION "testing"

#endif


#endif // DEFINITION_H
