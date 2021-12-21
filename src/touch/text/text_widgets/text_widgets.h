#ifndef TEXT_WIDGETS_H
#define TEXT_WIDGETS_H

#include <QWidget>
#include <QByteArray>
#include "zip.h"

#include "touch/datastruct/point.h"

#define D_X 0.0
#define D_Y 0.0
#define D_XF 60.0
#define D_YF 20.0

/*
 * this point contain all information for the text
 * into all file
*/
struct pointText{
    double x, y; /* top left of the rect */

    int size; /* size of the text */
    
    struct colore_s color; /* color of the text */

    /* for saving reason is better to have memory serialize */
    QByteArray text;
    Q_ALWAYS_INLINE QPointF toPoint() const;
};

Q_ALWAYS_INLINE QPointF pointText::toPoint() const
{
    return QPointF(x, y);
}

namespace Ui {
class text_widgets;
}

class text_widgets : public QWidget
{
    Q_OBJECT

public:
    explicit text_widgets(QWidget *parent, class TabletCanvas *canvas);
    ~text_widgets();

    bool isIn(const QPointF &point);

    TabletCanvas *parent;

    int saveData(zip_source_t *);
    int loadData(zip_file_t *);

    void createNew(const QPointF &point); /* create new point with text */

    void needToDraw(QPainter &painter);

private:
    QList<pointText> m_list;

    Ui::text_widgets *ui;
protected:
    bool event(QEvent *event) override;
};

#endif // TEXT_WIDGETS_H
