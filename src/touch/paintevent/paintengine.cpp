#include "paintengine.h"
#include "../../utils/threadcount.h"

PaintEngine::PaintEngine()
{
    const uint thread = threadCount::count();
    uint i;
    PaintEnginePrivate *ob;

    for(i = 0; i < thread; i++){
        ob = new PaintEnginePrivate;
        this->m_thread.append(ob);
    }
}
