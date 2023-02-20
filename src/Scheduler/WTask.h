#pragma once

#include <QtGlobal>
#include <QWidget>
#include <QRunnable>

class WTask:    public QObject,
                public QRunnable
{
    Q_OBJECT
public:
    explicit WTask(QObject *parent = nullptr);
    ~WTask() override = default;
    void run() override = 0;

    Q_DISABLE_COPY(WTask);

signals:
    /**
     * @ensures task == this
     * */
    void finished(WTask *task);
};
