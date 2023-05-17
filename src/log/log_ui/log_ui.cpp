#include "log_ui.h"
#include <QSettings>
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

#include <pthread.h>
#include <QDir>
#include <QFile>

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
    if(WFile::readFile(data, this->getCurrentPosition()) < 0)
        return false;
    return true;
}

WString log_ui::adjust_path(const WString &str) const
{
    WString tmp = str;

    tmp.replace(QChar(':'), QChar('_'));
    tmp.replace(QChar(' '), QChar('_'));

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
    QFile file(this->getCurrentPosition());
    WString tmp;

    if(m_permi != permi::enable){
        qDebug() << "It's not possibile write to log because of permission";
        qDebug() << "Message: " << stringa;
    }

    if(var == log_ui::info)
        tmp = "info: " + stringa;
    else if(var == log_ui::error_internal)
        tmp = "internal error: " + stringa;
    else
        tmp = "CRITIC ERROR: " + stringa;

    WMutexLocker _(mutex);

    if (un(!file.open(QIODevice::Append))) {
        m_permi = permi::error;
        return;
    }

    log_ui::addTime(tmp);

    file.write(WString("\n%1 --- %2").arg(tmp).arg(stringa).toUtf8());

    file.close();
}

void log_ui::print(FILE *fp, const WByteArray &str)
{
    fprintf(fp, "%s", str.constData());
}

void log_ui::addTime(WString &message)
{
    message = current_day_string() + current_time_string() + message;
}

WByteArray log_ui::getNameLog()
{
    return this->_nameLog;
}

void log_ui::saveData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);

    setting.setValue(KEY_LOG_POSITION_DEFINE, this->m_permi);

    setting.endGroup();
}


int log_ui::loadData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);

    WString _pos;

    this->m_permi = static_cast<permi>(setting.value(KEY_LOG_POSITION_DEFINE, permi::enable).toInt());

    if(m_permi == permi::disable)
        return 0;

    this->_nameLog += WString("writernote-log-%1-%2.txt")
            .arg(adjust_path(current_day_string()))
            .arg(adjust_path(current_time_string()))
            .toUtf8();

    setting.endGroup();
    return 0;
}

log_ui * NAME_LOG_EXT;
