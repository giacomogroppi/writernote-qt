#ifndef WIDGET_PARENT_H
#define WIDGET_PARENT_H

#include <QWidget>
#include <QList>
#include "../struct_last_file.h"

class element_ui;

namespace Ui {
class widget_parent;
}

class widget_parent : public QWidget
{
    Q_OBJECT

public:
    explicit widget_parent(QWidget *parent = nullptr, last_file *ref = nullptr);
    ~widget_parent();
    void updateList();

private:
    Ui::widget_parent *ui;
    QList<element_ui *> m_element;
    last_file *m_last_file;


    void clean();
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_PARENT_H
