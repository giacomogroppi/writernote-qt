#pragma once

#include "core/WPainter/WPainter.h"
#include "core/PointF.h"
#include "core/WVector.h"
#include "core/WListFast.h"

#include "currenttitle/document.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"

constexpr bool debugSquare = false;

class Square: public WObject,
              public SquareMethod
{
public:
    explicit Square(WObject *parent,
                    Fn<void()> hideProperty,
                    Fn<void(const PointF& point, ActionProperty signal)> showProperty,
                    Fn<Document &()> getDoc,
                    Fn<void(UpdateEvent event)> callUpdate);
    ~Square() override;

    int getType() const final;
    static constexpr int type();

    void reset() override;

private:
    Fn<void(UpdateEvent event)> _callUpdate;
    void needRefreshPrivate(UpdateEvent event) override;
};

inline constexpr int Square::type()
{
    return 4;
}
