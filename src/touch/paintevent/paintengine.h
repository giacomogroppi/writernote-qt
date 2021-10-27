#ifndef PAINTENGINE_H
#define PAINTENGINE_H

#include "paintengineprivate.h"
#include "../datastruct/datastruct.h"

class PaintEngine
{
private:
    QList<PaintEnginePrivate *> m_thread;
public:
    PaintEngine();
    void run(datastruct *data);
};

inline void PaintEngine::run(datastruct *data)
{
    uint i;
    const uint len = data->lengthPage();
}

#endif // PAINTENGINE_H
