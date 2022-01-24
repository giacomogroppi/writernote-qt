#include "touch/datastruct/datastruct.h"
#include "touch/multi_thread_data.h"
#include "pthread.h"
#include "audioplay/audioplay.h"

#define DATASTRUCT_THREAD_MAX 16

static QVector<int> index_last;

struct DatastructNewView{
    page     *m_page;
    QList<stroke>   *m_stroke;
    bool            is_mod;

    int base;

    int last_time;
    int new_time;

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
    for(const auto &index : pos){
        const stroke &stroke = page->atStroke(index);
        page->drawStroke(stroke, pos_audio);
    }
}

void *__search_new_view(void *__data)
{
    DataPrivateMuThread *_data = (DataPrivateMuThread *)__data;
    DatastructNewView *_private_data = (DatastructNewView *)_data->extra;
    page *_page = _private_data->m_page;
    constexpr int nullable_audio = -1;
    QVector<int> pos = index_last;

    if(unlikely(_private_data->is_mod == true)){
        pos.clear();

        __search_for_stroke(_data, _private_data->last_time, pos, _page);
    }

    __search_for_stroke(_data, _private_data->new_time, index_last, _page);

    // block other thread
    pthread_mutex_lock(_private_data->mutex);

    // draw the old stroke with their color
    drawStroke(_page, pos, nullable_audio);

    // draw the new stroke
    _page->drawForceColorStroke(index_last, nullable_audio, COLOR_NULL);
    drawStroke(_page, index_last, _private_data->new_time);

    // release other thread
    pthread_mutex_unlock(_private_data->mutex);

    return NULL;
}

void datastruct::newViewAudio(int lastTime, int newTime)
{
    int index, create, len;

    index = this->getFirstPageVisible();
    len = this->lengthPage();

    DataPrivateMuThread dataThread[DATASTRUCT_THREAD_MAX];
    pthread_t thread[DATASTRUCT_THREAD_MAX];

    DatastructNewView extra;

    extra.mutex = &changeAudioMutex;
    extra.new_time = newTime;
    extra.last_time = lastTime;

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
