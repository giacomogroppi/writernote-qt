#ifndef ELEMENT_UI_H
#define ELEMENT_UI_H

#include <QWidget>
#include <QListWidgetItem>
#include "../struct_last_file.h"


class element_ui : public QWidget
{
    Q_OBJECT

public:
    explicit element_ui(QWidget *parent = nullptr);
    ~element_ui();

    void setData(last_file *);
    inline QListWidgetItem *getItem()
    { return this->item; }
    void decrease();
private:
    QListWidgetItem *item;
    void set_main();

    last_file *m_data = NULL;

};

#endif // ELEMENT_UI_H
