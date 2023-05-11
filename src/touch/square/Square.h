#pragma once

#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QList>

#include "currenttitle/document.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"

constexpr bool debugSquare = false;

class Square: public QObject,
              public SquareMethod
{
    Q_OBJECT
public:
    explicit Square(QObject *parent,
                    std::function<void()> hideProperty,
                    std::function<void(const QPointF& point, ActionProperty signal)> showProperty,
                    std::function<Document &()> getDoc,
                    std::function<void()> callUpdate);
    ~Square() override;

    int getType() const final;
    static constexpr int type();

    void reset() override;

private:
    std::function<void()> _callUpdate;
    void needRefreshPrivate() override;
};

inline constexpr int Square::type()
{
    return 4;
}
