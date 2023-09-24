#pragma once

#include "utils/WCommonScript.h"
#include "Scheduler/WObject.h"
#include "core/ByteArray/WByteArray.h"
#include "core/String/WString.h"
#include "core/Path/WPath.h"

#define NAME_LOG_EXT log_write


class log_ui : public WObject
{
public:
    explicit log_ui(WObject * parent, WPath positionForLog);
    ~log_ui();

    enum type_write: int{
        critic_error,
        info,
        error_internal,
        possible_bug
    };

    void write(const WString &message,
               enum type_write type);

    void print(FILE *fp, const WByteArray &str);
    auto getData(WByteArray & str) -> bool;
    auto getCurrentPosition() const -> WPath;
private:
    auto adjust_path(const WString &str) const -> WString;
    auto check_str(const WString &str) const -> bool;
    static void addTime(WString &message);

    enum permi: int {
        error, /* error */
        disable, /* log disable */
        enable /* ok */
    };

    [[nodiscard]]
    auto getNameLog() const -> WByteArray;

    permi m_permi = permi::enable;

    const WPath _positionLog;
    WByteArray _nameLog;

    void saveData();
    int loadData();
};

inline auto log_ui::getCurrentPosition() const -> WPath
{
    W_ASSERT(_nameLog.size());
    return this->_positionLog / _nameLog;
}

extern log_ui *NAME_LOG_EXT;
//log_ui::type_write::
#define LOG(message, mode) NAME_LOG_EXT->write(message, log_ui::type_write::mode)
#define LOG_CONDITION(condition, message, mode) if((condition)) NAME_LOG_EXT->write(message, mode)

// debug info
#ifdef DEBUGINFO

#   define DEBUG_INFO_CALLER_SINGLE(caller) const char *caller
#   define DEBUG_INFO_CALLER_MULTIPLE(caller) ,DEBUG_INFO_CALLER_SINGLE(caller)

/* mode is "start" or "stop" */
#   define DEBUG_INFO_CALL_CALLER_PRIVATE(caller, mode, thisFunction) LOG(WString(caller)+ " call " + WString(thisFunction) + " " + mode, log_ui::type_write::__caller)
#   define DEBUG_INFO_CALL_CALLER(caller, thisFunction) DEBUG_INFO_CALL_CALLER_PRIVATE(caller, "start", thisFunction)
#   define DEBUG_INFO_END_CALLER(caller, thisFunction) DEBUG_INFO_CALL_CALLER_PRIVATE(caller, "stop",thisFunction)

#   define CALLER_MULTIPLE(caller) ,caller
#   define CALLER_SINGLE(caller) caller

#else
#   define DEBUG_INFO_CALLER_SINGLE(caller)
#   define DEBUG_INFO_CALLER_MULTIPLE(caller)

#   define DEBUG_INFO_CALL_CALLER(caller, thisFunction) ;
#   define DEBUG_INFO_END_CALLER(caller, thisFunction) ;

#   define CALLER_MULTIPLE(caller) ;
#   define CALLER_SINGLE(caller) ;

#endif

// example
/*void foo(int arg1, int arg2, char **argv DEBUG_INFO_CALLER_MULTIPLE(caller)){
    DEBUG_INFO_CALL_CALLER(caller, "name this function");
    DEBUG_INFO_END_CALLER(caller, "name this function");
}

void main(void){
    foo(arg1, arg2, (char **)argv, CALLER("main"));
}
*/
