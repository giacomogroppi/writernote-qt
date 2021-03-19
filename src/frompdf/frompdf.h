#ifndef FROMPDF_H
#define FROMPDF_H

class currenttitle_class;

#include <QString>
#include <QList>
#include <QImage>

class frompdf
{
private:
    currenttitle_class *m_data;

    void add(QString &);

    QList<QImage> m_image;

public:
    static bool isvalid(QString &pos);

    frompdf(currenttitle_class *);
};

#endif // FROMPDF_H
