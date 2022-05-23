#pragma once

#include "zip.h"
#include "utils/common_script.h"
#include <QString>
#include <QObject>

class PrivateStatus{
private:
#define S_OPEN BIT(1)
#define D_AVAILABLE BIT(2)

    int _status = ~(S_OPEN | D_AVAILABLE);
public:
    PrivateStatus() = default;
    ~PrivateStatus() = default;

    force_inline bool isDataAvailable() const
    {
        return _status & D_AVAILABLE;
    }

    force_inline bool is_open() const
    {
        return _status & S_OPEN;
    }

};

class WZip
{
private:
    void *_data = NULL;
    QString path;
    PrivateStatus _status;

    bool openZip(const QString &path);

public:
    WZip(const QString &path);
    ~WZip() = default;

    WZip &operator=(WZip &&other) = delete;
    WZip &operator=(WZip &other)  = delete;
    WZip &operator=(WZip other)   = delete;
};
