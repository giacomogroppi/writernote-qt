#include "SquareMethod.h"
#include "core/core.h"
#include "touch/square/square.h"
#include "touch/tabletcanvas.h"

SquareMethod::SquareMethod()
{
}

bool SquareMethod::touchBegin(const QPointF &point, double size, struct Document &doc)
{
    constexpr auto not_used ManageStartdebugSquare = false;
    auto *_square = core::get_canvas()->_square;

    if(_square->somethingInBox()){
        WDebug(ManageStartdebugSquare, "Somethininbox");
        _square->initPointMove(point);
    }
    else{
        WDebug(ManageStartdebugSquare, "not in box");
        _square->initPoint(point);
    }
}

bool SquareMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    auto *_square = core::get_canvas()->_square;

    _square->isMoving();

    if (_square->somethingInBox()) {
        W_ASSERT(_square->get_first_point().isSet());

        /** a questo punto può muovere di un delta x e y */
        _square->move(point);
    } else {
        /**
        * it means that the user not select anything
        * in the past
        */
        _square->updatePoint(point);
    }
}

int SquareMethod::touchEnd(const QPointF &point, class Document &doc)
{
    auto *_canvas = core::get_canvas();
    auto *_square = _canvas->_square;
    bool done = _square->somethingInBox();

    if(done){
        _square->reset();
    }

    if(!done){
        _square->find();
    }

    _square->endMoving(_canvas);
}
