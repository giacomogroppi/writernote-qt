#ifndef CONTROLL_LOG_H
#define CONTROLL_LOG_H

#include <QObject>

class controll_log : public QObject
{
    Q_OBJECT
public:
    explicit controll_log(QObject *parent = nullptr);
    ~controll_log();

    enum type_write: int{
        critic_error,
        info,
        error_internal
    };

    void write(QString &,
               enum controll_log::type_write = type_write::info);

    void write(const char *,
               enum controll_log::type_write = type_write::info);

    void write(QString ,
               enum controll_log::type_write = type_write::info);



signals:
private:

    enum errorWriting: unsigned int{
        ok_writing,
        error_append,
        not_exist

    };

    errorWriting write_(const char *, const char *);
    errorWriting manageWrite(const char *,
                             QString &,
                             enum controll_log::type_write);

    bool ovverrideError(enum errorWriting res);

    enum e_setOut{
        ref,
        ok
    };

    e_setOut setOut(QString &);

    int loadData();
    void saveData();

    QString pos;
    enum permi: int {
        error, /* error */
        disable, /* log disable */
        enable /* ok */
    };
    enum permi m_permi;

};

#endif // CONTROLL_LOG_H
