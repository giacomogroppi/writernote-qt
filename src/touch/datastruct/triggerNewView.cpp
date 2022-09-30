#include "touch/datastruct/datastruct.h"
#include "touch/multi_thread_data.h"
#include "pthread.h"
#include "audioplay/audioplay.h"

#define DATASTRUCT_THREAD_MAX 16

struct DatastructNewView{
    page     *m_page;
    QList<Stroke>   *m_stroke;
    int base;

    int time;

    pthread_mutex_t *mutex;
};

void __search_for_stroke(DataPrivateMuThread *data, int pos_audio, QVector<int> &save, page *page)
{
    W_ASSERT(data);
    W_ASSERT(page);

    for(; data->from < data->to; data->from ++){
        if(page->atStroke(data->from).getPosizioneAudio() == pos_audio){
            save.append(data->from);
        }
    }
}

void drawStroke(page *page, QVector<int> &pos, int pos_audio)
{
    for(const auto &index : qAsConst(pos)){
        const Stroke &stroke = page->atStroke(index);
        page->drawStroke(stroke, pos_audio);
    }
}

void *__search_new_view(void *__data)
{
    DataPrivateMuThread *_data = (DataPrivateMuThread *)__data;
    DatastructNewView *_private_data = (DatastructNewView *)_data->extra;
    page *_page = _private_data->m_page;
    QVector<int> _index;

    __search_for_stroke(_data, _private_data->time - 1, _index, _page);

    // block other thread
    pthread_mutex_lock(_private_data->mutex);

    // draw the new stroke
    drawStroke(_page, _index, _private_data->time - 1);

    // release other thread
    pthread_mutex_unlock(_private_data->mutex);

    return NULL;
}

void datastruct::newViewAudio(int newTime)
{
    int index, create, len;

    index = this->getFirstPageVisible();
    len = this->lengthPage();

    DataPrivateMuThread dataThread[DATASTRUCT_THREAD_MAX];
    pthread_t thread[DATASTRUCT_THREAD_MAX];

    DatastructNewView extra;

    if(unlikely(newTime == 0)){
        for(page &_page : _page){
            _page.triggerRenderImage(newTime, true);
        }
    }
    qDebug() << "Call with time" << newTime;
    extra.mutex = &_changeAudioMutex;
    extra.time = newTime;

    for(; index < len; index ++){
        extra.m_page = (page *)&at(index);

        create = DataPrivateMuThreadInit(dataThread, &extra, DATASTRUCT_THREAD_MAX, extra.m_page->lengthStroke(), 0);
        if(unlikely(create == 1)){
            __search_new_view(&dataThread[0]);
            continue;
        }

        START_THREAD(thread, dataThread, create, __search_new_view);
        JOIN_THREAD(thread, create);
    }
}
