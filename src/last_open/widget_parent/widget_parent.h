#ifndef WIDGET_PARENT_H
#define WIDGET_PARENT_H

#include <QWidget>
#include <QList>
#include "../struct_last_file.h"

class last_open;
class element_ui;

namespace Ui {
class widget_parent;
}

class widget_parent : public QWidget
{
    Q_OBJECT

public:
    explicit widget_parent(QWidget *parent = nullptr, last_file *ref = nullptr,
                           const bool showOnlyName = false, last_open *parent_sec = nullptr);
    ~widget_parent();
    void updateList();
    void decrease(const int index);

private:
    Ui::widget_parent *ui;
    last_open *parent;
    QList<element_ui *> m_element;
    last_file *m_last_file;


    void clean();
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_PARENT_H
