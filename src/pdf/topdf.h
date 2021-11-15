#ifndef TOPDF_H
#define TOPDF_H
#include "../touch/datastruct/datastruct.h"
#include "../mainwindow.h"
#include "../touch/tabletcanvas.h"

class topdf
{
private:
    Document *data = nullptr;
    const QString *path;

    void translate();

    MainWindow *parent;

    void draw(QPainter &painter, double m,
              int size_orizzontale, int size_verticale, const bool withPdf);

public:
    ~topdf(){
        delete data;
    };

    topdf(const QString &path);
    bool createpdf(const bool withPdf);

    inline void copy(Document *datastruct_){
        this->data = new Document;
        Document::copy(*datastruct_, *data);
    }
};

inline void topdf::draw(QPainter &painter, double m, int size_orizzontale, int size_verticale, const bool withPdf)
{
    DataPaint dataPaint = {
        .withPdf = withPdf,
        .IsExportingPdf = false,
        .m = m,
        .size = QSize(size_orizzontale, size_verticale),
        .parent = nullptr,
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };
    TabletCanvas::load(painter, this->data, dataPaint);
}

#endif // TOPDF_H
