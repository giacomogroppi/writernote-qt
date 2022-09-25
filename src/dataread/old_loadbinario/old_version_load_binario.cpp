#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "utils/posizione_binario.h"
#include "utils/common_error_definition.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "touch/datastruct/page.h"

#ifdef ALL_VERSION

/* this is the data struct used for converting to the new format */
struct point_old_ver_7{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;

    int idtratto;
    int page; /* indicates which page the point is on */

    size_t createControll() const;
    bool isIdUser() const;
    QPointF toQPointF(const double delta) const;
};

struct point_last{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;
    int idtratto;
};

/*
 * version 2 and 3
*/
int xmlstruct::loadbinario_0(WZip &reader)
{
    struct colore_last{
        int colore[NCOLOR];
    };

    /* this is the data structure we expect to read from the file */
    struct point_last_ver_2_3{
        double m_x, m_y, rotation;
        float m_pressure;
        int m_posizioneaudio;
        struct colore_last m_color;
        int idtratto;

        size_t createControll() const;
        bool isIdUser() const;
    };

    QList<double> pos_foglio;
    QList<point_last> point;
    struct zip_stat st;
    int i, len;
    uint k;

    point_last_ver_2_3 point_lettura;
    point_last point_append;

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

    zip_file_t *f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr) return ERROR;

    SOURCE_READ_GOTO(f, &len, sizeof(int));

    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &point_lettura, sizeof(point_lettura));

        point_append.idtratto = point_lettura.idtratto;
        for(k = 0; k < NCOLOR; ++k){
            point_append.m_color.colore[k] = point_lettura.m_color.colore[k];
        }

        point_append.m_posizioneaudio = point_lettura.m_posizioneaudio;
        point_append.m_x = point_lettura.m_x;
        point_append.m_y = point_lettura.m_y;
        point_append.m_pressure = point_lettura.m_pressure;
        point_append.rotation = point_lettura.rotation;

        point.append(point_append);
    }

    double valoretemp;

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        pos_foglio.append(valoretemp);
    }

    {
        long double zoom;
        SOURCE_READ_GOTO(f, &zoom, sizeof(long double));
        _doc->datatouch->_zoom = zoom;
    }
    zip_fclose(f);

    xmlstruct::decode0(_doc, point, pos_foglio);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

/* version 4 5 6 */
int xmlstruct::loadbinario_1(class WZip &zip)
{
    struct zip_stat st;
    int i, len;
    struct point_last temp_point;
    double valoretemp;
    WZipReaderSingle reader(&zip, 0);

    QList<point_last> __tmp;
    QList<double> pos_foglio;

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;


    if(reader.read_object(len) < 0)
        return ERROR;

    for(i = 0; i < len; i++){
        if(reader.read_object(temp_point) < 0)
            return ERROR;

        __tmp.append(temp_point);
    }

    if(reader.read_object(len) < 0)
        return ERROR;

    for(i = 0; i < len; i++){
        if(reader.read_object(valoretemp) < 0)
            return ERROR;
        pos_foglio.append(valoretemp);
    }

    {
        long double zoom;

        static_assert(sizeof(zoom) == sizeof(long double));
        if(reader.read_object(zoom) < 0)
            return ERROR;

        _doc->datatouch->_zoom = (double)zoom;
    }


    xmlstruct::decode0(_doc, __tmp, pos_foglio);
    return OK;
}


static void scaleAll(QList<point_last> &point, const QPointF &translation)
{
    int i;
    cint len = point.length();
    point_last *ref;

    for(i = 0; i< len; i++){
        ref = &point.operator[](i);
        ref->m_x -= translation.x();
        ref->m_y -= translation.y();
    }
}

static QPointF bigger(const QList<point_last> &point)
{
    QPointF max(0.0, 0.0);
    uint i;
    const point_last *ref;
    const uint len = point.length();

    for(i = 0; i < len; i++ ){
        ref = &point.at(i);
        if(ref->m_x > max.x())
            max.setX(ref->m_x);
        if(ref->m_y > max.y())
            max.setY(ref->m_y);
    }

    return max;
}

static void adjastPDF(QList<point_last> &point, QList<double> &pos_foglio)
{
    uint i, len;
    const QPointF currentSize = bigger(point);
    const double CorrectProportions = double(page::getHeight())/double(page::getWidth());
    const uint lenPage = pos_foglio.length();

    const double shouldBe = currentSize.x()*CorrectProportions*lenPage;
    const double tmp = shouldBe/currentSize.y();

    len = point.length();
    for(i = 0; i < len; i++){
        point.operator[](i).m_y *= tmp;
    }

    len = pos_foglio.length();
    for(i = 0; i < len; i++){
        pos_foglio.operator[](i) *= tmp;
    }
}

static void adjastZoom(QList<point_last> &point, QList<double> &pos_foglio)
{
    const QPointF big = bigger(point);
    const double Width = page::getWidth();
    const double delta = Width / big.x();

    point_last *ref;

    uint i;
    uint len = point.length();

    for(i = 0; i < len; i++){
        ref = &point.operator[](i);
        ref->m_x *= delta;
        ref->m_y *= delta;
    }

    len = pos_foglio.length();
    for(i = 0; i < len; i++){
        pos_foglio.operator[](i) *= delta;
    }

    adjastPDF(point, pos_foglio);
}

static int old_which_sheet(
        const point_last    &point,
        QVector<page>         &ListPage)
{
    const page *page;
    uint counterPage, len;
    len = ListPage.length();

    for(counterPage = 0; counterPage < len; counterPage++){
        page = &ListPage.at(counterPage);
        if(page->currentHeight() >= point.m_y && page->minHeight() <= point.m_y){
            return counterPage;
        }
    }

    Q_ASSERT(false);
    return -1;
}

static void toVersion7(point_old_ver_7 &to, point_last &from)
{
    to.idtratto = from.idtratto;
    memcpy(&to.m_color, &from.m_color, sizeof(to.m_color));
    to.m_posizioneaudio = from.m_posizioneaudio;
    to.m_pressure = from.m_pressure;
    to.m_x = from.m_x;
    to.m_y = from.m_y;
    to.rotation = from.rotation;
}

/*
 * the function translates data written
 * with version 0 of binary old
*/
void xmlstruct::decode0(
        Document                    *data,
        QList<struct point_last>    &point,
        QList<double>               &pos_foglio)
{
    const point_last firstPoint = point.takeFirst();
    const int lenPage           = pos_foglio.length();
    const QPointF translation(firstPoint.m_x, firstPoint.m_y);

    QList<QList<point_old_ver_7>> pointForAppend;

    int i = 0, counterPage;

    scaleAll(point, translation);
    adjastZoom(point, pos_foglio);
    data->datatouch->setPointFirstPage(translation);
    data->datatouch->_zoom = 1.0;

    /* create the sheet */
    for(counterPage = 0; counterPage <= lenPage; counterPage ++){
        data->datatouch->newPage(n_style::white);
        pointForAppend.append(QList<point_old_ver_7> ());
    }

    W_ASSERT(pointForAppend.length() == data->datatouch->lengthPage());

    for(i = 0; i < point.length(); i++){
        point_last pp = point.at(i);

        pp.m_pressure *= 0.2;

        const int which = old_which_sheet(pp, data->datatouch->_page);

        {
            point_old_ver_7 __append;
            toVersion7(__append, pp);
            __append.page = which;

            pointForAppend.operator[](which).append(__append);
        }

    }

    xmlstruct::decode1(data, pointForAppend);
}

void xmlstruct::decode1(Document *doc, QList<QList<struct point_old_ver_7>> &__page)
{
    int counterPage;
    cint lenPage = __page.length();

    if(doc->datatouch->lengthPage() != __page.length()){
        doc->datatouch->newPage(doc->datatouch->lengthPage());
    }

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        for(int counterPoint = 0; counterPoint < __page.at(counterPage).length(); counterPoint ++){
            QList<point_old_ver_7> &ListPrivate = __page.operator[](counterPage);
            stroke stroke;
            int id;

            {
                const point_old_ver_7 &tmp = ListPrivate.at(counterPoint);
                id = tmp.idtratto;
                stroke.setMetadata(tmp.m_posizioneaudio, tmp.m_color);
            }

            for(; counterPoint < ListPrivate.length() &&
                ListPrivate.at(counterPoint).idtratto == id;
                counterPoint ++) {
                const point_old_ver_7 &tmpRef = ListPrivate.at(counterPoint);

                point_s TmpAppend;
                TmpAppend._x = tmpRef.m_x;
                TmpAppend._y = tmpRef.m_y;

                if (unlikely(id < 0)) {
                    continue;
                } else {
                    stroke.append(TmpAppend, tmpRef.m_pressure);
                }
            }
            
            if(likely(id >= 0)){
                doc->datatouch->appendStroke(stroke);
            }
        }
    }

    doc->datatouch->_pointFirstPage = QPointF(0.0, 0.0);
}

/* versione 7 */
int xmlstruct::loadbinario_2(class WZip &zip)
{
    size_t controll, newControll;
    int i, len, lenPage, counterPage;
    struct point_old_ver_7 temp_point;
    QList<QList<point_old_ver_7>> pointAppend;
    double init[2];
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    static_assert(sizeof(init) == sizeof(double) * 2);
    /* point first page */
    if(reader.read_object(init) < 0)
        return ERROR;
    this->_doc->datatouch->setPointFirstPage(QPointF(init[0], init[1]));

    /* page len */
    if(reader.read_object(lenPage) < 0)
        return ERROR;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        if(reader.read_object(len) < 0)
            return ERROR;

        /* we add a new page */
        pointAppend.append(QList<point_old_ver_7> ());

        for(i = 0; i < len; i++){
            if(reader.read_object(temp_point) < 0)
                return ERROR;
            pointAppend.operator[](counterPage).append(temp_point);
        }
    }

    static_assert(sizeof(_doc->datatouch->_zoom) == sizeof(double));
    if(reader.read_object(_doc->datatouch->_zoom) < 0)
        return ERROR;

    if(reader.read_object(controll) < 0)
        return ERROR;


    _doc->datatouch->triggerNewView(-1, true);
    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    xmlstruct::decode1(_doc, pointAppend);

    return OK;
}

__old int xmlstruct::loadbinario_3(class WZip &zip, int ver_stroke)
{
    size_t controll, newControll;
    int lenPage, counterPage;
    datastruct *data = _doc->datatouch;
    double init[2];
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    static_assert(sizeof(init) == sizeof(double) * 2);
    static_assert(sizeof(_doc->datatouch->_zoom) == sizeof(double));

    /* point first page */
    if(reader.read_object(init) < 0)
        return ERROR;
    this->_doc->datatouch->setPointFirstPage(QPointF(init[0], init[1]));

    /* page len */
    if(reader.read_object(lenPage) < 0)
        return ERROR;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        /* we add a new page */
        data->newPage(n_style::white);

        if(data->at_mod(counterPage).load(reader, ver_stroke) != OK)
            return ERROR;
    }

    if(reader.read_object(_doc->datatouch->_zoom) < 0)
        return ERROR;

    if(reader.read_object(controll) < 0)
        return ERROR;

    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;
}

#endif //ALL_VERSION
