#include "dataread/xmlstruct.h"
#include "datawrite/source_read_ext.h"
#include "sheet/style_struct.h"
#include "utils/posizione_binario.h"
#include "utils/common_error_definition.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WZip.h"
#include "core/WZipReaderSingle.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

/* this is the data struct used for converting to the new format */
struct point_old_ver_7{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct WColor m_color;

    int idtratto;
    int page; /* indicates which page the point is on */

    size_t createControll() const;
    bool isIdUser() const;
    PointF toPointF(const double delta) const;
};

struct point_last{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct WColor m_color;
    int idtratto;
};

/**
 * version 2 and 3
*/
int xmlstruct::loadbinario_0(WZip &zip)
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

    WListFast<double> pos_foglio;
    WListFast<point_last> point;
    WZipReaderSingle reader(&zip, 0);
    int i, len;
    uint k;

    W_ASSERT(reader.is_data_available() == false);

    point_last_ver_2_3 point_lettura;
    point_last point_append;

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    if(reader.read_object(len) < 0)
        return ERROR;

    for(i=0; i<len; i++){
        if(reader.read_object(point_lettura) < 0)
            return ERROR;

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

    if(reader.read_object(len) < 0)
        return ERROR;

    for(i=0; i < len; i++){
        if(reader.read_object(valoretemp) < 0)
            return ERROR;

        pos_foglio.append(valoretemp);
    }

    {
        long double zoom;
        if(reader.read_object(zoom) < 0)
            return ERROR;

        _doc->_zoom = zoom;
    }

    xmlstruct::decode0(_doc, point, pos_foglio);

    return OK;
}

/* version 4 5 6 */
int xmlstruct::loadbinario_1(class WZip &zip)
{
    int i, len;
    struct point_last temp_point;
    double valoretemp;
    WZipReaderSingle reader(&zip, 0);

    WListFast<point_last> __tmp;
    WListFast<double> pos_foglio;

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

        _doc->_zoom = (double)zoom;
    }


    xmlstruct::decode0(_doc, __tmp, pos_foglio);
    return OK;
}


static void scaleAll(WListFast<point_last> &point, const PointF &translation)
{
    int i;
    cint len = point.size();
    point_last *ref;

    for(i = 0; i< len; i++){
        ref = &point.operator[](i);
        ref->m_x -= translation.x();
        ref->m_y -= translation.y();
    }
}

static PointF bigger(const WListFast<point_last> &point)
{
    PointF max(0.0, 0.0);
    uint i;
    const point_last *ref;
    const uint len = point.size();

    for(i = 0; i < len; i++ ){
        ref = &point.at(i);
        if(ref->m_x > max.x())
            max.setX(ref->m_x);
        if(ref->m_y > max.y())
            max.setY(ref->m_y);
    }

    return max;
}

static void adjastPDF(WListFast<point_last> &point, WListFast<double> &pos_foglio)
{
    uint i, len;
    const PointF currentSize = bigger(point);
    const double CorrectProportions = double(Page::getHeight()) / double(Page::getWidth());
    const uint lenPage = pos_foglio.size();

    const double shouldBe = currentSize.x()*CorrectProportions*lenPage;
    const double tmp = shouldBe/currentSize.y();

    len = point.size();
    for(i = 0; i < len; i++){
        point.operator[](i).m_y *= tmp;
    }

    len = pos_foglio.size();
    for(i = 0; i < len; i++){
        pos_foglio.operator[](i) *= tmp;
    }
}

static void adjastZoom(WListFast<point_last> &point, WListFast<double> &pos_foglio)
{
    const PointF big = bigger(point);
    const double Width = Page::getWidth();
    const double delta = Width / big.x();

    point_last *ref;

    uint i;
    uint len = point.size();

    for(i = 0; i < len; i++){
        ref = &point.operator[](i);
        ref->m_x *= delta;
        ref->m_y *= delta;
    }

    len = pos_foglio.size();
    for(i = 0; i < len; i++){
        pos_foglio.operator[](i) *= delta;
    }

    adjastPDF(point, pos_foglio);
}

static int old_which_sheet(
        const point_last    &point,
        WVector<Page>         &ListPage)
{
    const Page *page;
    uint counterPage, len;
    len = ListPage.size();

    for(counterPage = 0; counterPage < len; counterPage++){
        page = &ListPage.at(counterPage);
        if(page->currentHeight() >= point.m_y and page->minHeight() <= point.m_y){
            return counterPage;
        }
    }

    W_ASSERT(false);
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
        WListFast<struct point_last>    &point,
        WListFast<double>               &pos_foglio)
{
    const point_last firstPoint = point.takeFirst();
    const int lenPage           = pos_foglio.size();
    const PointF translation(firstPoint.m_x, firstPoint.m_y);

    WListFast<WListFast<point_old_ver_7>> pointForAppend;

    int i = 0, counterPage;

    scaleAll(point, translation);
    adjastZoom(point, pos_foglio);
    data->setPointFirstPage(translation);
    data->_zoom = 1.0;

    /* create the sheet */
    for(counterPage = 0; counterPage <= lenPage; counterPage ++){
        data->newPage(n_style::white);
        pointForAppend.append(WListFast<point_old_ver_7> ());
    }

    W_ASSERT(pointForAppend.length() == data->lengthPage());

    for(i = 0; i < point.size(); i++){
        point_last pp = point.at(i);

        pp.m_pressure *= 0.2;

        const int which = old_which_sheet(pp, data->_page);

        {
            point_old_ver_7 __append;
            toVersion7(__append, pp);
            __append.page = which;

            pointForAppend.operator[](which).append(__append);
        }

    }

    xmlstruct::decode1(data, pointForAppend);
}

void xmlstruct::decode1(Document *doc, WListFast<WListFast<struct point_old_ver_7>> &__page)
{
    int counterPage;
    cint lenPage = __page.size();

    if(doc->lengthPage() != __page.size()){
        doc->newPage(doc->lengthPage());
    }

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        for(int counterPoint = 0; counterPoint < __page.at(counterPage).size(); counterPoint ++){
            WListFast<point_old_ver_7> &ListPrivate = __page.operator[](counterPage);
            std::shared_ptr<StrokeNormal> stroke(new StrokeNormal);
            int id;

            {
                const point_old_ver_7 &tmp = ListPrivate.at(counterPoint);
                id = tmp.idtratto;
                stroke->setMetadata(tmp.m_posizioneaudio, tmp.m_color);
            }

            for (; counterPoint < ListPrivate.size() and
                ListPrivate.at(counterPoint).idtratto == id;
                counterPoint ++) {
                const point_old_ver_7 &tmpRef = ListPrivate.at(counterPoint);

                PointF TmpAppend;
                TmpAppend.rx() = tmpRef.m_x;
                TmpAppend.ry() = tmpRef.m_y;

                if (un(id < 0)) {
                    continue;
                } else {
                    stroke->append(TmpAppend, tmpRef.m_pressure);
                }
            }
            
            if (likely(id >= 0)) {
                doc->appendStroke(stroke);
            }
        }
    }

    doc->_pointFirstPage = PointF(0.0, 0.0);
}

/** versione 7 */
int xmlstruct::loadbinario_2(class WZip &zip)
{
    size_t controll, newControll;
    int i, len, lenPage, counterPage;
    struct point_old_ver_7 temp_point;
    WListFast<WListFast<point_old_ver_7>> pointAppend;
    double init[2];
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    static_assert(sizeof(init) == sizeof(double) * 2);
    /* point first page */
    if(reader.read_object(init) < 0)
        return ERROR;
    this->_doc->setPointFirstPage(PointF(init[0], init[1]));

    /* page len */
    if(reader.read_object(lenPage) < 0)
        return ERROR;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        if(reader.read_object(len) < 0)
            return ERROR;

        /* we add a new page */
        pointAppend.append(WListFast<point_old_ver_7> ());

        for(i = 0; i < len; i++){
            if(reader.read_object(temp_point) < 0)
                return ERROR;
            pointAppend.operator[](counterPage).append(temp_point);
        }
    }

    static_assert(sizeof(_doc->_zoom) == sizeof(double));
    if(reader.read_object(_doc->_zoom) < 0)
        return ERROR;

    if(reader.read_object(controll) < 0)
        return ERROR;


    _doc->triggerNewView(-1, true);
    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    xmlstruct::decode1(_doc, pointAppend);

    return OK;
}

/* old */ int xmlstruct::loadbinario_3(class WZip &zip, int ver_stroke)
{
    size_t controll, newControll;
    int lenPage, counterPage;
    double init[2];
    WZipReaderSingle reader(&zip, 0);

    if(!zip.openFileInZip(NAME_BIN))
        return ERROR;

    static_assert(sizeof(init) == sizeof(double) * 2);
    static_assert(sizeof(_doc->_zoom) == sizeof(double));

    /* point first page */
    if(reader.read_object(init) < 0)
        return ERROR;
    this->_doc->setPointFirstPage(PointF(init[0], init[1]));

    /* page len */
    if(reader.read_object(lenPage) < 0)
        return ERROR;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        /* we add a new page */
        _doc->newPage(n_style::white);

        if(_doc->at_mod(counterPage).load(reader, ver_stroke) != OK)
            return ERROR;
    }

    if(reader.read_object(_doc->_zoom) < 0)
        return ERROR;

    if(reader.read_object(controll) < 0)
        return ERROR;

    newControll = _doc->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    return OK;
}
