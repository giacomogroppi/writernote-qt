#include "compressivideo.h"

#include <stdlib.h>

int compressvideo(QString *posizionefrom, QString *to, int fps, QString compressione)
{
    /* ffmpeg -i input.mp4 -vcodec libx265 -crf 28 output.mp4*/
    QString comando = "ffmpeg -i " + *posizionefrom +  " -vcodec " + compressione + " -crf " + QString::number(fps) + " " + *to;

    return system(comando.toUtf8().constData());
}
