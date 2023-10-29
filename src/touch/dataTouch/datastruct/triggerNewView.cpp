#include "touch/dataTouch/datastruct/DataStruct.h"
#include "touch/multi_thread_data.h"
#include "pthread.h"
#include "audioplay/audioplay.h"

#define DATASTRUCT_THREAD_MAX 16

struct DatastructNewView{
    Page            *m_page;
    int             time;
    WMutex          &mutex;
};

void __search_for_stroke(DataPrivateMuThread *data, int pos_audio, WVector<int> &save, Page *page)
{
    W_ASSERT(data);
    W_ASSERT(page);

    for(; data->from < data->to; data->from ++){
        if(page->atStroke(data->from).getPositionAudio() == pos_audio){
            save.append(data->from);
        }
    }
}

void drawStroke(Page *page, WVector<int> &pos, int pos_audio)
{
    page->drawStroke(pos, pos_audio);
}

void *__search_new_view(void *__data)
{
    auto *_data = static_cast<DataPrivateMuThread *>(__data);
    auto *_private_data = static_cast<DatastructNewView *>(_data->extra);
    Page *_page = _private_data->m_page;
    WVector<int> _index;

    __search_for_stroke(_data, _private_data->time - 1, _index, _page);

    // block other thread
    _private_data->mutex.lock();

    // draw the new stroke
    drawStroke(_page, _index, _private_data->time - 1);

    // release other thread
    _private_data->mutex.unlock();

    return nullptr;
}

void DataStruct::newViewAudio(int newTime)
{
    int index, create, len;

    index = this->getFirstPageVisible();
    len = this->lengthPage();

    DataPrivateMuThread dataThread[DATASTRUCT_THREAD_MAX];
    pthread_t thread[DATASTRUCT_THREAD_MAX];

    DatastructNewView extra = {
            .m_page = nullptr,
            .time = newTime,
            .mutex = _changeAudioMutex
    };

    if (newTime == 0) {
        for(auto &ref : _page){
            ref.triggerRenderImage(newTime, true);
        }
    }
    WDebug(true, "Call with time" << newTime);

    for(; index < len; index ++){
        extra.m_page = (Page *)&at(index);

        create = DataPrivateMuThreadInit(dataThread, &extra, DATASTRUCT_THREAD_MAX, extra.m_page->lengthStroke(), 0);
        if (create == 1) {
            __search_new_view(&dataThread[0]);
            continue;
        }

        START_THREAD(thread, dataThread, create, __search_new_view);
        JOIN_THREAD(thread, create);
    }
}
