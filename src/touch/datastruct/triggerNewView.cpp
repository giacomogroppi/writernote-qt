#include "touch/datastruct/datastruct.h"
#include "touch/multi_thread_data.h"
#include "pthread.h"

#define DATASTRUCT_THREAD_MAX 16

struct DatastructNewView{
    QList<page>     *m_page;
    QList<stroke>   *m_stroke;
};

void *__search_new_view(void *__data)
{
    std::abort();
}

void datastruct::newViewAudio(int lastTime, int newTime)
{
    int index = this->getFirstPageVisible();
    int len = this->lengthPage();
    QList<QVector<int>> position;

    DataPrivateMuThread dataThread[DATASTRUCT_THREAD_MAX];
    pthread_t thread[DATASTRUCT_THREAD_MAX];

    DatastructNewView extra;

    for(; index < len; index ++){
        int create, i;

        extra.m_page = (QList<page> *)&at(index);

        create = DataPrivateMuThreadInit(dataThread, &extra, DATASTRUCT_THREAD_MAX, extra.m_page->length());
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
