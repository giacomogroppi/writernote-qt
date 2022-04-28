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

    void Show(const QPoint &pos, int index);

private:
    int _index;
    QPushButton *_del;
    QPushButton *_copy;
    QPushButton *_paste;

private slots:
    void del();
    void paste();
    void copy();

signals:
    void ClickCommand(int signal);
    void ClickCopy(int index);
    void ClickPaste(int index);

};

#endif // LIST_OPTIONS_H
