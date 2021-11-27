#include "../xmlstruct.h"
#include "../../datawrite/source_read_ext.h"
#include "../../utils/posizione_binario.h"
#include "../../utils/common_error_definition.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"

#ifdef ALL_VERSION

/* this is the data struct used for converting to the new format */
struct point_old{
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

int xmlstruct::loadbinario_0(zip_t *z){
    struct colore_last{
        int colore[NCOLOR];
    };


    /* this is the data structure we expect to read from the file */
    struct point_last{
        double m_x, m_y, rotation;
        float m_pressure;
        int m_posizioneaudio;
        struct colore_last m_color;
        int idtratto;

        size_t createControll() const;
        bool isIdUser() const;
    };



    QList<double> pos_foglio;
    QList<point_old> point;
    struct zip_stat st;
    int i, len;
    uint k;

    struct point_last point_lettura;
    struct point_old temp_point;

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

    zip_file_t *f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr) return ERROR;

    SOURCE_READ_GOTO(f, &len, sizeof(int));

    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &point_lettura, sizeof(struct point_last));

        temp_point.idtratto = point_lettura.idtratto;
        for(k=0; k<NCOLOR; ++k){
            temp_point.m_color.colore[k] = point_lettura.m_color.colore[k];
        }

        temp_point.m_posizioneaudio = point_lettura.m_posizioneaudio;
        temp_point.m_x = point_lettura.m_x;
        temp_point.m_y = point_lettura.m_y;
        temp_point.m_pressure = point_lettura.m_pressure;
        temp_point.rotation = point_lettura.rotation;
        temp_point.page = -1;

        point.append(temp_point);
    }

    double valoretemp;

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        pos_foglio.append(valoretemp);
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    zip_fclose(f);

    xmlstruct::decode0(currenttitle, point, pos_foglio);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

struct point_last{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;
    int idtratto;

    size_t createControll() const;
    bool isIdUser() const;
};

static size_t createControllOldVersion(const QList<point_last> &point)
{
    uint i;
    const uint len = point.length();
    size_t controll = 0;
    for(i = 0; i < len; i++){
        controll += point.at(i).createControll();
    }

    return controll;
}

int xmlstruct::loadbinario_1(struct zip *z){

    struct zip_stat st;
    size_t controll;
    int i, len;
    zip_file_t *f;
    struct point_last temp_point;
    double valoretemp;

    QList<point_old> __tmp;
    point_old __point;

    QList<point_last> point;
    QList<double> pos_foglio;

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

     f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr) return false;

    SOURCE_READ_GOTO(f, &len, sizeof(int));

    for(i=0; i<len; i++){
        SOURCE_READ_GOTO(f, &temp_point, sizeof(struct point_last));
        point.append(temp_point);
    }

    SOURCE_READ_GOTO(f, &len, sizeof(int));
    for(i=0; i < len; i++){
        SOURCE_READ_GOTO(f, &valoretemp, sizeof(double));
        pos_foglio.append(valoretemp);
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(long double));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);
    {
        const size_t newControll = createControllOldVersion(point);
        if(controll != newControll){
            return ERROR_CONTROLL;
        }
    }
    for(const auto & ref : qAsConst(point)){
        memcpy(&__point.m_color, &ref.m_color, sizeof(colore_s));
        __point.idtratto = ref.idtratto;
        __point.m_posizioneaudio = ref.m_posizioneaudio;
        __point.m_pressure = ref.m_pressure;
        __point.m_x = ref.m_x;
        __point.m_y = ref.m_y;
        __point.rotation = ref.rotation;

        __tmp.append(__point);
    }

    xmlstruct::decode0(currenttitle, __tmp, pos_foglio);
    return OK;

    free_:
    zip_fclose(f);

    return ERROR;
}


static void scaleAll(QList<point_old> &point, const QPointF &translation)
{
    uint i;
    const uint len = point.length();
    point_old *ref;
    for(i=0; i<len; i++){
        ref = &point.operator[](i);
        ref->m_x -= translation.x();
        ref->m_y -= translation.y();
    }
}

static QPointF bigger(const QList<point_old> &point)
{
    QPointF max(0.0, 0.0);
    uint i;
    const point_old *ref;

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

static void adjastPDF(QList<point_old> &point, QList<double> &pos_foglio){
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

static void adjastZoom(QList<point_old> &point, QList<double> &pos_foglio)
{
    const QPointF big = bigger(point);
    const double Width = page::getWidth();
    const double delta = Width / big.x();

    point_old *ref;

    uint i;
    uint len = point.length();

    for(i = 0; i < len; i++){
        ref = &point.operator[](i);
        ref->m_x *= delta;
        ref->m_y *= delta;
    }

    len = pos_foglio.length();
    for(i=0; i<len; i++){
        pos_foglio.operator[](i) *= delta;
    }

    adjastPDF(point, pos_foglio);
}

int old_which_sheet(const point_old &point, QList<page> &ListPage)
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

void xmlstruct::decode0(Document *data, QList<point_old> &point, QList<double> &pos_foglio)
{
    const point_old firstPoint = point.takeFirst();
    const int lenPage = pos_foglio.length();
    const QPointF translation(firstPoint.m_x, firstPoint.m_y);
    point_old pp;
    QList<QList<point_old>> pointForAppend;

    int i = 0, counterPage, len;

    scaleAll(point, translation);
    adjastZoom(point, pos_foglio);
    data->datatouch->setPointFirstPage(translation);
    data->datatouch->zoom = 1.0;

    /* create the sheet */
    for(counterPage = 0; counterPage <= lenPage; counterPage ++){
        data->datatouch->newPage(n_style::white);
        pointForAppend.append(QList<point_old> ());
    }

    Q_ASSERT(pointForAppend.length() == data->datatouch->lengthPage());

    for(i = 0; i < point.length(); i++){
        pp = point.at(i);
        pp.m_pressure *= 0.2;
        const int which = old_which_sheet(pp, data->datatouch->m_page);
        pointForAppend.operator[](which).append(pp);
    }

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        for(i = 0, len = pointForAppend.at(counterPage).length(); i < len; i++){

            const int which = old_which_sheet(pointForAppend.at(counterPage).at(i), data->datatouch->m_page);

            if(which != pointForAppend.at(counterPage).at(i).page){
                point_old point = pointForAppend.at(counterPage).at(i);
                point.page = which;

                pointForAppend.operator[](counterPage).removeAt(i);

                pointForAppend.operator[](which).append(point);
            }
        }
    }

    xmlstruct::decode(data, pointForAppend);
}

void xmlstruct::decode(Document *doc, QList<QList<point_old>> &page)
{
    int counterPage, lenPage;
    lenPage = page.length();

    {
        int currentLen = doc->datatouch->lengthPage();
        if(currentLen != page.length()){
            while(currentLen > 0){
                doc->datatouch->newPage(n_style::white);
                currentLen --;
            }
        }
    }

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        for(int counterPoint = 0; counterPage < page.at(counterPage).length(); counterPage ++){
            stroke stroke;
            const point_old &tmp = page.at(counterPage).at(counterPoint);

            const int id = tmp.idtratto;

            stroke.setMetadata(tmp.page, id, tmp.m_posizioneaudio, tmp.m_color);

            for(; counterPoint < page.at(counterPage).length() && page.at(counterPage).at(counterPoint).idtratto == id; counterPoint ++){
                const point_old &tmpRef = page.operator[](counterPage).takeAt(counterPoint);

                point_s TmpAppend;
                TmpAppend.m_x = tmpRef.m_x;
                TmpAppend.m_y = tmpRef.m_y;
                TmpAppend.pressure = tmpRef.m_pressure;
                stroke.append(TmpAppend, true);
            }

            doc->datatouch->appendStroke(stroke);

        }
    }
}

size_t point_last::createControll() const{
    static size_t data;
    static int i;

    data = 0;
    data += m_x;
    data += m_y;
    data += m_pressure;
    data += idtratto;

    for(i=0; i<NCOLOR; ++i){
        data += m_color.colore[i];
    }

    data += this->rotation;
    data += this->m_posizioneaudio;

    return data;
}


int xmlstruct::loadbinario_2(struct zip *z){
    struct zip_stat st;
    size_t controll, newControll;
    int i, len, lenPage, counterPage;
    zip_file_t *f;
    struct point_old temp_point;
    QList<QList<point_old>> pointAppend;
    double init[2];

    zip_stat_init(&st);
    zip_stat(z, NAME_BIN, 0, &st);

     f = zip_fopen(z, NAME_BIN, 0);

    if(f == nullptr) return ERROR;

    /* point first page */
    SOURCE_READ_GOTO(f, init, sizeof(double)*2);
    this->currenttitle->datatouch->setPointFirstPage(QPointF(init[0], init[1]));

    /* page len */
    SOURCE_READ_GOTO(f, &lenPage, sizeof(lenPage));
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        SOURCE_READ_GOTO(f, &len, sizeof(len));

        /* we add a new page */
        pointAppend.append(QList<point_old> ());

        for(i=0; i<len; i++){
            SOURCE_READ_GOTO(f, &temp_point, sizeof(temp_point));
            pointAppend.operator[](counterPage).append(temp_point);
        }
    }

    SOURCE_READ_GOTO(f, &this->currenttitle->datatouch->zoom, sizeof(this->currenttitle->datatouch->zoom));

    SOURCE_READ_GOTO(f, &controll, sizeof(size_t));

    zip_fclose(f);

    currenttitle->datatouch->triggerNewView(-1, true);
    newControll = currenttitle->createSingleControll();

    if(controll != newControll)
        return ERROR_CONTROLL;

    xmlstruct::decode(currenttitle, pointAppend);

    return OK;

    free_:
    zip_fclose(f);
    return ERROR;
}

#endif //ALL_VERSION
