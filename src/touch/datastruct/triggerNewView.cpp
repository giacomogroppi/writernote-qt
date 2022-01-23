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
};

void __search_for_stroke(DataPrivateMuThread *data, int pos_audio, QVector<int> save, page *page)
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
    QVector<int> pos;

    if(unlikely(_private_data->is_mod == true)){
        __search_for_stroke(_data, _private_data->last_time, pos, _page);
        drawStroke(_page, pos, _private_data->last_time);
    }else{
        drawStroke(_page, index_last, _private_data->last_time);
    }



    return NULL;
}

void datastruct::newViewAudio(int lastTime, int newTime)
{
    int index = this->getFirstPageVisible();
    int len = this->lengthPage();
    QList<QVector<int>> position;

    DataPrivateMuThread dataThread[DATASTRUCT_THREAD_MAX];
    pthread_t thread[DATASTRUCT_THREAD_MAX];

    DatastructNewView extra;

    extra.new_time = newTime;
    extra.last_time = lastTime;

    for(; index < len; index ++){
        int create, i;

        extra.m_page = (page *)&at(index);

        create = DataPrivateMuThreadInit(dataThread, &extra, DATASTRUCT_THREAD_MAX, extra.m_page->lengthStroke(), 0);
        if(unlikely(create == 1)){
            __search_new_view(&dataThread[0]);
            continue;
        }
        for(i = 0; i < create; i ++){
            pthread_create(&thread[i], NULL, __search_new_view, &dataThread[i]);
        }
        for(i = 0; i < create; i++){
            pthread_join(thread[i], NULL);
        }
        std::abort();
    }
}
