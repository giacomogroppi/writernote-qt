#ifndef CONTROLL_LOG_H
#define CONTROLL_LOG_H

#include <QObject>

class controll_log : public QObject
{
    Q_OBJECT
public:
    explicit controll_log(QObject *parent = nullptr);
    ~controll_log();


    void write(QString &);
    void write(const char *);
    void write(QString );



signals:
private:

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
    permi m_permi;

};

#endif // CONTROLL_LOG_H
