#include "log_ui.h"
#include "utils/setting_define.h"
#include "utils/path/get_path.h"
#include "utils/time/current_time.h"
#include "utils/mostra_explorer/mostra_explorer.h"
#include "utils/slash/slash.h"
#include "utils/time/current_time.h"
#include "utils/WCommonScript.h"
#include "core/WFile.h"
#include "core/WMutex.h"
#include "core/WMutexLocker.h"
#include "file/File.h"
#include "core/WOptionSetting/WOptionSettings.h"
#include <pthread.h>
#include "utils/time/current_time.h"

static WMutex mutex;

log_ui::log_ui(WObject *parent, const WByteArray &positionForLog)
    : WObject(parent)
    , _positionLog(positionForLog)
{
    this->loadData();
}

log_ui::~log_ui()
{
    this->saveData();
}

bool log_ui::getData(WByteArray &data)
{
    if(WFile::readFile(data, this->getCurrentPosition().constData()) < 0)
        return false;
    return true;
}

WString log_ui::adjust_path(const WString &str) const
{
    WString tmp = str;

    tmp.replace(':', '_');
    tmp.replace(' ', '_');

    return tmp;
}

bool log_ui::check_str(const WString &str) const
{
    const auto res = str.indexOf(":");
    if(res < 0)
        return true;
    return false;
}

void log_ui::write(const WString &stringa, log_ui::type_write var)
{
    WFile file(this->getCurrentPosition(), WFile::WFileWrite);
    WString tmp;

    if(m_permi != permi::enable){
        WDebug(true, "It's not possible to write log because of permission. Log: " << stringa.toUtf8().toStdString());
    }

    if(var == log_ui::info)
        tmp = "info: " + stringa;
    else if(var == log_ui::error_internal)
        tmp = "internal error: " + stringa;
    else
        tmp = "CRITIC ERROR: " + stringa;

    WMutexLocker _(mutex);

    if (!file.open(WFile::WFileAppend)) {
        m_permi = permi::error;
        return;
    }

    log_ui::addTime(tmp);

    const auto msg = WString("\n%1 --- %2").arg(tmp).arg(stringa).toUtf8();
    file.write(msg.constData(), msg.size());

    file.close();
}

void log_ui::print(FILE *fp, const WByteArray &str)
{
    fprintf(fp, "%s", str.constData());
}

void log_ui::addTime(WString &message)
{
    message = WDate::now().toString() + WTime::now().toString() + message;
}

WByteArray log_ui::getNameLog()
{
    return this->_nameLog;
}

void log_ui::saveData()
{
    WOptionSettings settings;
    settings.begin();

    settings.setValue(KEY_LOG_POSITION_DEFINE, this->m_permi);

    settings.save();
}


int log_ui::loadData()
{
    WOptionSettings settings;
    settings.begin();

    WString _pos;

    this->m_permi = static_cast<permi>(settings.value(KEY_LOG_POSITION_DEFINE, permi::enable).toInt());

    if(m_permi == permi::disable)
        return 0;

    this->_nameLog += WString("writernote-log-%1-%2.txt")
            .arg(adjust_path(WDate::now().toString()))
            .arg(adjust_path(WTime::now().toString()))
            .toUtf8();

    settings.save();
    return 0;
}

log_ui * NAME_LOG_EXT;
