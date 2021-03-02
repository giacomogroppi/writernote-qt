#ifndef TEXT_WIDGETS_H
#define TEXT_WIDGETS_H

#include <QWidget>
#include "zip.h"

#include "../../datastruct/datastruct.h"

/*
 * this point contain all information for the text
 * into all file
*/
struct pointText{
    double x, y; /* top left of the rect */
    double xf, yf; /* button right of the rect */

    int size; /* size of the text */
    int alfa;
    struct colore_s color; /* color of the text */
};

namespace Ui {
class text_widgets;
}

class text_widgets : public QWidget
{
    Q_OBJECT

public:
    explicit text_widgets(QWidget *parent = nullptr, datastruct *data = nullptr);
    ~text_widgets();

    bool isIn(QPointF);

    datastruct *m_data;

    int saveData(zip_source_t *);
    int loadData(zip_file_t *);

    void createNew(QPointF ); /* create new point with text */

private:
    QList<pointText> m_lista;

    Ui::text_widgets *ui;
};

#endif // TEXT_WIDGETS_H
