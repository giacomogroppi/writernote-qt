#ifndef LIST_OPTIONS_H
#define LIST_OPTIONS_H

#include <QWidget>
#include <QPushButton>

class list_options : public QWidget
{
    Q_OBJECT
public:
    enum signal{
        DEL = 0,
    };

    explicit list_options(QWidget *parent = nullptr);
    ~list_options();

    void Show(const QPoint &pos);

private:
    QPushButton *_del;

private slots:
    void del();

signals:
    void ClickCommand(int signal);

protected:
    bool event(QEvent *event) override;
};

#endif // LIST_OPTIONS_H
