#include "Rubber.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "utils/setting_define.h"
#include "core/WOptionSetting/WOptionSettings.h"

constexpr auto not_used rubber_debug = false;

Rubber::Rubber(WObject *parent) :
    WObject(parent),
    RubberMethod(_type_gomma, _size_gomma)
{
    this->reset();

    WOptionSettings settings;
    settings.begin();

    _size_gomma = settings.value(KEY_RUBBER_SIZE, DEFAULT_GOMMA_SIZE).toInt();
    _type_gomma = static_cast<RubberMethod::type_rubber>(
        settings.value(KEY_RUBBER_TYPE, RubberMethod::type_rubber::total).toInt()
    );

    settings.save();
}

Rubber::~Rubber()
{
    WOptionSettings settings;
    settings.begin();

    settings.setValue(KEY_RUBBER_SIZE, _size_gomma);
    settings.setValue(KEY_RUBBER_TYPE, _type_gomma);

    settings.save();
}

int Rubber::getType() const
{
    return Rubber::type();
}

void Rubber::setRubberTotal()
{
    this->_type_gomma = RubberMethod::type_rubber::total;
    W_EMIT_0(onRubberChange);
}

void Rubber::setRubberPartial()
{
    _type_gomma = RubberMethod::type_rubber::partial;

    W_EMIT_0(onRubberChange);
}

