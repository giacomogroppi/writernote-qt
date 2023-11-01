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

void drawStroke(Page *page, WVector<int> &pos, int pos_audio)
{
    page->drawStroke(pos, pos_audio);
}

struct dPrivate {
    /**
     * since this that is shared between all the instances of datastruct when you need to call this set
     * of tasks you need to synchronize with \link accessToDrawMutex
     * */
    WMutex accessToDrawMutex;
    WVector<Scheduler::Ptr<WTask>> tasks;
    WVector<DataPrivateMuThread> data;

    ~dPrivate() {
        W_ASSERT(Scheduler::isStop());
        tasks.forAll(&Scheduler::Ptr<WTask>::release);
    }
} dataStructAudioTask;

class DataStructNewViewTask: public WTask
{
private:
    DataPrivateMuThread &_data;
    int position_audio;

    auto searchForStroke(Page &page, int position_audio) -> WVector<int>;
public:
    explicit DataStructNewViewTask(DataPrivateMuThread &data);

    void run() final;
};

DataStructNewViewTask::DataStructNewViewTask(DataPrivateMuThread &data)
    : WTask{nullptr, WTask::NotDeleteLater}
    , _data(data)
    , position_audio(0)
{

}

auto DataStructNewViewTask::searchForStroke(Page &page, int positionAudio) -> WVector<int>
{
    WVector<int> result;

    for (; _data.from < _data.to; _data.from ++) {
        if (page.atStroke(_data.from).getPositionAudio() == positionAudio) {
            result.append(_data.from);
        }
    }

    return result;
}

void DataStructNewViewTask::run()
{
    auto *private_data = static_cast<DatastructNewView *>(_data.extra);
    Page &page = *private_data->m_page;

    const auto index = searchForStroke(page, private_data->time - 1);

    // block others thread
    WMutexLocker guard (private_data->mutex);

    // draw the new stroke
    page.drawStroke(index, private_data->time - 1);
}

void DataStruct::init()
{
    const auto threadCreated = threadCount::count();

    for (int i = 0; i < threadCreated; i++) {
        dataStructAudioTask.data.append(DataPrivateMuThread());
        dataStructAudioTask.tasks.append(
                Scheduler::Ptr<WTask>(
                        new DataStructNewViewTask(dataStructAudioTask.data[i])
                )
        );
    }

    Page::init();
}

void DataStruct::newViewAudio(int newTime)
{
    int index;

    index = this->getFirstPageVisible();

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
    WDebug(debug, "Call with time" << newTime);

    WMutexLocker guard(dataStructAudioTask.accessToDrawMutex);

    for(; index < lengthPage(); index ++){
        extra.m_page = (Page *)&at(index);

        auto create = DataPrivateMuThreadInit(dataStructAudioTask.data, &extra, DATASTRUCT_THREAD_MAX, extra.m_page->lengthStroke(), 0);

        dataStructAudioTask.tasks.refMidConst(0, create).forAll(&Scheduler::addTaskGeneric);
        dataStructAudioTask.tasks.refMidConst(0, create).forAll(&WTask::join);
    }
}
