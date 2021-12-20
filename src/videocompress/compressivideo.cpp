#include "compressivideo.h"
#include "utils/dialog_critic/dialog_critic.h"

#include <stdlib.h>

int compressvideo(QString *posizionefrom, QString *to, int fps, QString compressione)
{
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    Q_UNUSED(posizionefrom);
    Q_UNUSED(to);
    Q_UNUSED(fps);
    Q_UNUSED(compressione);
    user_message("Video compress is not available on your platform");
#else
    /* ffmpeg -i input.mp4 -vcodec libx265 -crf 28 output.mp4*/
    QString comando = "ffmpeg -i " + *posizionefrom +  " -vcodec " + compressione + " -crf " + QString::number(fps) + " " + *to;

    return system(comando.toUtf8().constData());
#endif
}
