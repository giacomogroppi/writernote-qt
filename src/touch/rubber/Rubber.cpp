#include "Rubber.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include <QSettings>
#include "utils/setting_define.h"

constexpr auto not_used rubber_debug = false;

Rubber::Rubber(QObject *parent) :
    QObject(parent),
    RubberMethod(_type_gomma, _size_gomma)
{
    this->reset();

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    _size_gomma = setting.value(KEY_RUBBER_SIZE, DEFAULT_GOMMA_SIZE).toInt();
    _type_gomma = static_cast<RubberMethod::type_rubber>(
            setting.value(KEY_RUBBER_TYPE, RubberMethod::type_rubber::total).toInt()
    );

    setting.endGroup();
}

Rubber::~Rubber()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_RUBBER);

    setting.setValue(KEY_RUBBER_SIZE, _size_gomma);
    setting.setValue(KEY_RUBBER_TYPE, _type_gomma);

    setting.endGroup();
}

void Rubber::setRubberTotal()
{
    this->_type_gomma = RubberMethod::type_rubber::total;
    emit onRubberChange();
}

void Rubber::setRubberPartial()
{
    _type_gomma = RubberMethod::type_rubber::partial;

    emit this->onRubberChange();
}

