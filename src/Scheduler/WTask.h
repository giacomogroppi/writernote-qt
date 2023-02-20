#pragma once

#include <QtGlobal>
#include <QWidget>

class WTask: public QObject {
    Q_OBJECT
public:
    explicit WTask(QObject *parent = nullptr);
    ~WTask() override = default;

    virtual int doTask() = 0;
};
