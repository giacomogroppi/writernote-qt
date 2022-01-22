#include "page.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include <QPainter>
#include "utils/time/current_time.h"
#include <QPainterPath>
#include "utils/common_error_definition.h"
#include "datawrite/source_read_ext.h"
#include "time.h"
#include "utils/common_script.h"
#include "touch/multi_thread_data.h"

#define PAGE_THREAD_MAX 16
#define Define_PEN(pen) QPen pen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
#define TEMP_COLOR QColor::fromRgb(105, 105, 105, 255)
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

static inline double widthToPressure(double v);
static void setStylePrivate(bool &fast, n_style res, style_struct_S &style);
static void drawLineOrizzontal(stroke &stroke, const style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(stroke &stroke, const style_struct_S &style,
                            const double &last, double &deltay, const double height_p);

static force_inline void __initImg(QImage &img)
{
    img = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);
}

page::page(const int count, const n_style style)
{
    this->count = count;
    this->IsVisible = true;
    drawNewPage(style);
    this->mergeList();
}

void page::drawNewPage(n_style __style)
{
    bool fast = false;
    double deltax, deltay, ct_del;
    struct style_struct_S style;
    cdouble width_p    = this->getWidth();
    cdouble height_p   = this->getHeight();
    cdouble last = (count-1)*page::getHeight();

    stroke &stroke1 = this->m_stroke_writernote[0];
    stroke &stroke2 = this->m_stroke_writernote[1];

    setStylePrivate(fast, __style, style);

    if(likely(fast)){
        style.colore.fromColor(TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    stroke1.setMetadata(-1, style.colore);
    stroke2.setMetadata(-1, style.colore);

    if(style.nx <= 0)
        style.nx = 1;

    if(style.ny <= 0)
        style.ny = 1;

    deltax = height_p / (double)style.nx;
    deltay = width_p / (double)style.ny;

    ct_del = deltax;

    drawLineOrizzontal( stroke1, style, last, deltax, width_p, ct_del);
    drawLineVertical(   stroke2,     style, last, deltay, height_p);

    stroke1.__setPressureFirstPoint(    widthToPressure(style.thickness));
    stroke2.__setPressureFirstPoint(      widthToPressure(style.thickness));
}

void page::swap(
        QList<stroke>       &list,
        const QVector<int>  &pos,
        int                 flag)
{
    int i, index;
    QRectF area;

#ifdef DEBUGINFO
    if(unlikely(!is_order(pos))){
        qDebug() << "List not order" << __FILE__ << __FUNCTION__;
    }
#endif

    if(!(flag & PAGE_SWAP_TRIGGER_VIEW)){
        i = pos.length();
        for(i--; i >= 0; i--){
            index = pos.at(i);
            this->swap(list, index, index+1);
        }
        return;
    }

    for(cint ref : pos){
        const stroke & stroke = atStroke(ref);
        list.append(stroke);
    }

    area = this->get_size_area(pos);

    this->removeAndDraw(-1, pos, area);
}

/*
 * this function mantain the item already in list
*/
void page::swap(QList<stroke> & list,
                int             from,
                int             to)
{
    W_ASSERT(from >= to);
    DO_IF_DEBUG(
    int drop = 0;
    QList<int> itemDrop;
    );

    for(to --; from <= to; to --){
        list.append(m_stroke.takeAt(to));

        DO_IF_DEBUG_ENABLE(debugPage, drop ++);
        DO_IF_DEBUG_ENABLE(debugPage, itemDrop.append(to));
    }

    DO_IF_DEBUG_ENABLE(debugPage,
        qDebug() << "Page::swap" << this->count - 1 << drop << "Item drop, list" << itemDrop;
            )
}

/* the list should be order */
void page::removeAt(const QVector<int> &pos)
{
    int i;
    if(unlikely(!is_order(pos))){
        DO_IF_DEBUG(std::abort());
        order((QVector<int> &)(pos));
    }

    i = pos.length();
    for(i--; i >= 0; i--){
        this->removeAt(i);
    }
}

void page::append(const QList<stroke> &stroke)
{
    for(const auto & __tmp : qAsConst(stroke)){
        this->append(__tmp);
    }
}

void page::drawStroke(
        QPainter        &painter,
        const stroke    &stroke,
        QPen            &m_pen,
        const QColor    &color) const
{
    QPointF lastPoint, pointDraw;
    const QPainterPath *path;

    constexpr bool measureTime = false;
    constexpr bool debColor = true;

    cint page = this->count - 1;

    Q_UNUSED(measureTime);
    Q_UNUSED(debColor);

    m_pen.setColor(color);
    m_pen.setWidthF(TabletCanvas::pressureToWidth(stroke.getPressure() / 2.00) * PROP_RESOLUTION);

    if(unlikely(!painter.isActive())){
#ifdef DEBUGINFO
        qDebug() << "page::drawStroke" << "painter not active";
#else
        log_write->write("Painter not active", log_ui::possible_bug);
#endif
        return;
    }

    if(unlikely(color == COLOR_NULL)){
        m_pen.setWidthF(m_pen.widthF() * 1.5);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
    }

    if(stroke.constantPressure()){
        EXEC_TIME_IF_DEBUG("page::drawStroke() getQPainterPath()", measureTime,
            path = &stroke.getQPainterPath(page);
        )

        EXEC_TIME_IF_DEBUG("page::drawStroke(), strokePath", measureTime,
            painter.strokePath(*path, m_pen);
        )

    }else{
        int counterPoint;
        const int lenPoint = stroke.length();
        const int refCounter = this->count - 1;

        lastPoint = at_translation(stroke.at(0), refCounter).toQPointF(PROP_RESOLUTION);

        for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
            EXEC_TIME_IF_DEBUG("page::drawStroke() take point", measureTime,
                const point_s point = at_translation(stroke.at(counterPoint), refCounter);
                pointDraw = point.toQPointF(PROP_RESOLUTION);

                m_pen.setWidthF(TabletCanvas::pressureToWidth(point.pressure / 2.00) * PROP_RESOLUTION);

            )

            EXEC_TIME_IF_DEBUG("page::drawStroke() draw", measureTime,
                painter.setPen(m_pen);

                painter.drawLine(lastPoint, pointDraw);
            )

            lastPoint = pointDraw;
        }
    }

    if(color == COLOR_NULL){
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }
}

struct page_thread_data{
    QVector<int>      * to_remove;
    pthread_mutex_t   * append;
    QPainter          * painter;
    QList<stroke>     * m_stroke;
    int                 m_pos_ris;
    const page        * parent;
};

/*The only way to draw in thread safe on a qpainter is to draw an image,
 *  as it is very fast compared to drawing the stroke, first we draw
 *  all the strokes on the images, residing in the thread stack stacks,
 *  and then we draw them on top the original painter
*/
void * __page_load(void *__data)
{
    struct DataPrivateMuThread * _data = (struct DataPrivateMuThread *)__data;
    struct page_thread_data *extra = (struct page_thread_data *)_data->extra;
    QImage img;
    Define_PEN(m_pen);
    pthread_mutex_t *mutex = extra->append;
    int m_pos_ris = extra->m_pos_ris;
    QPainter painter;
    const page *page = extra->parent;

    __initImg(img);
    painter.begin(&img);

    for(; _data->from < _data->to; _data->from ++){
        const auto &ref = extra->m_stroke->at(_data->from);
        if(unlikely(ref.isEmpty())){

            pthread_mutex_lock(mutex);
            extra->to_remove->append(_data->from);
            pthread_mutex_unlock(mutex);

            continue;
        }

        const QColor color = ref.getColor(
            (likely(ref.getPosizioneAudio()) > m_pos_ris) ? 1 : 4
        );

        page->drawStroke(painter, ref, m_pen, color);
    }

    painter.end();

    // the source image has the same size as img
    pthread_mutex_lock(mutex);
    extra->painter->drawImage(img.rect(), img, img.rect());
    pthread_mutex_unlock(mutex);

    return NULL;
}

void page::drawEngine(
        QPainter        &painter,
        QList<stroke>   &List,
        int             m_pos_ris,
        bool            *changeSomething,
        cbool           use_multi_thread)
{
    int i, threadCount;

    QVector<int> to_remove;
    pthread_mutex_t append;

    pthread_t thread[PAGE_THREAD_MAX];
    struct DataPrivateMuThread threadData[PAGE_THREAD_MAX];
    struct page_thread_data extraData;

    pthread_mutex_init(&append, NULL);

    extraData.append        = &append;
    extraData.painter       = &painter;
    extraData.to_remove     = &to_remove;
    extraData.m_stroke      = &List;
    extraData.m_pos_ris     = m_pos_ris;
    extraData.parent        = this;

    if(use_multi_thread){
        threadCount = DataPrivateMuThreadInit(threadData, &extraData, PAGE_THREAD_MAX, List.length(), ~DATA_PRIVATE_FLAG_SEM);

        for(i = 0; i < threadCount; i++){
            pthread_create(&thread[i], NULL, __page_load, &threadData[i]);
        }
        for(i = 0; i < threadCount; i++){
            pthread_join(thread[i], NULL);
        }
    }else{
        threadData->extra   = &extraData;
        threadData->from    = 0;
        threadData->to      = List.length();

        __page_load(&threadData[0]);
    }

    if(unlikely(to_remove.length())){
        order(to_remove);

        if(likely(changeSomething))
            *changeSomething = true;

        log_write->write("Stroke is empty", log_ui::type_write::possible_bug);

        this->removeAt(to_remove);
    }
}

inline void page::draw(
    QPainter    &painter,
    int         m_pos_ris,
    bool        all)
{
    auto list = strokeTmp.toList();
    bool changeSomething = true;

    if(unlikely(all)){
        this->drawEngine(painter, this->m_stroke, m_pos_ris, NULL, true);
    }

    this->drawEngine(painter, list, m_pos_ris, &changeSomething, false);

    if(unlikely(changeSomething)){
        this->strokeTmp = QVector<stroke>::fromList(list);
    }

    this->mergeList();
}

void page::mergeList()
{
    int i;
    const int len = this->strokeTmp.length();
    int index = m_stroke.length();

    for(i = 0; i < len; i++){
        const stroke &stroke = strokeTmp.at(i);

        if(likely(stroke.getColor(1).alpha() == 255)){
            m_stroke.append(stroke);
        }else{
            m_stroke.insert(0, stroke);
        }

        //Q_ASSERT(m_stroke.at(index).length() == strokeTmp.at(i).length());
        //Q_ASSERT(m_stroke.at(index).getId() == strokeTmp.at(i).getId());

        index ++;
    }

    strokeTmp.clear();
}

static void setStylePrivate(
    bool            &fast, 
    n_style         res, 
    style_struct_S  &style)
{
    if(res == n_style::empty){
        res = n_style::square;
    }

    if(res == n_style::line){
        fast = true;

        style.nx = TEMP_N_X;
        style.ny = 0;
    }
    else if(res == n_style::white){
        /* we set the color manually */
        style.colore.fromColor(Qt::black);

        style.nx = 0;
        style.ny = 0;
    }else{
        fast = true;

        style.nx = TEMP_SQUARE*(page::getHeight() / page::getWidth());
        style.ny = TEMP_SQUARE;
    }
}

static inline double widthToPressure(double v){
    return v/10.0;
}

static inline void drawLineOrizzontal(
    stroke                  &stroke,
    const style_struct_S    &style,
    const double            &last, 
    double                  &deltax, 
    const double            &width_p, 
    const double            &ct_del)
{
    int i;
    point_s point;

    W_ASSERT(ct_del > 0);

    for(i = 0; i < style.nx; i++){
        point.m_x = 20;
        point.m_y = last + deltax - 20;

        stroke.append(point);

        point.m_x = width_p - 20;
        stroke.append(point);

        deltax += ct_del;
    }
}

static inline void drawLineVertical(
    stroke                  &stroke,
    const style_struct_S    &style, 
    const double            &last, 
    double                  &deltay, 
    const double            height_p)
{    
    const double ct_del = deltay;
    int i;
    point_s point;

    W_ASSERT(height_p);
    W_ASSERT(ct_del);

    for(i = 0; i < style.ny; i++){
        point.m_x = deltay  - 20;
        point.m_y = last    + 20; /* corrisponde to 0 */

        stroke.append(point);

        point.m_y = height_p + last - 20;
        stroke.append(point);

        deltay += ct_del;
    }
}

void page::drawToImage(
    const QVector<int>  &index, 
    QImage              &img,
    cint                flag) const
{
    QPainter painter;
    Define_PEN(pen);

    if(flag & DR_IMG_INIT_IMG){
        __initImg(img);
    }
    
    painter.begin(&img);

    for (const int __index : index){
        const stroke &stroke = atStroke(__index);
        this->drawStroke(painter, stroke, pen, stroke.getColor());
    }

    painter.end();
}

bool page::userWrittenSomething() const
{
    return this->m_stroke.length();
}

bool page::initImg(bool flag)
{
    flag = imgDraw.isNull() || flag;
    if(flag)
        __initImg(imgDraw);

    return flag;
}

void page::decreseAlfa(const QVector<int> &pos, QPainter * painter, int decrese)
{
    int i = pos.length();
    uint color;
    Define_PEN(m_pen);

    for(i --; i >= 0; i--){
        stroke &stroke = atStrokeMod(pos.at(i));
        color = stroke.get_alfa();

        stroke.setAlfaColor(color / decrese);

        if(painter){
            this->drawStroke(*painter, stroke, m_pen, COLOR_NULL);
            this->drawStroke(*painter, stroke, m_pen, stroke.getColor());
        }
    }
}

/*
 * all --> indicates if all the points must be drawn from scratch, if false it is drawn over the old image
*/
void page::triggerRenderImage(int m_pos_ris, bool all)
{
    QPainter painter;
    all = initImg(all);

    painter.begin(&imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    this->draw(painter, m_pos_ris, all);

    painter.end();

    /*return;
    if(!imgDraw.save("~/Scrivania/tmp_foto/foto"+current_time_string()+".png", "PNG", 0))
        std::abort();*/
}

#define MakeCTRL(point) \
    if(datastruct::isinside(topLeft1, bottomRight1, point)) \
        return true;

static Q_ALWAYS_INLINE int __is_inside_square(
        const QRectF    &rect1,
        const QRectF    &rect2)
{
    const QPointF &topLeft1     = rect1.topLeft();
    const QPointF &bottomRight1 = rect1.bottomRight();

    MakeCTRL(rect2.bottomRight());
    MakeCTRL(rect2.bottomLeft());

    MakeCTRL(rect2.topRight());
    MakeCTRL(rect2.topLeft());

    return false;
}

static Q_ALWAYS_INLINE int is_inside_squade(
        const QRectF    &rect1,
        const QRectF    &rect2)
{
    return __is_inside_square(rect1, rect2) || __is_inside_square(rect2, rect1);
}

void page::removeAndDraw(
        int                 m_pos_ris,
        const QVector<int>  &pos,
        const QRectF        &area)
{
    int i = pos.length();
    this->drawForceColorStroke(pos, m_pos_ris, COLOR_NULL);

    for(i --; i >= 0; i --){
        removeAt(pos[i]);
    }

    drawIfInside(m_pos_ris, area);
}

void page::drawIfInside(int m_pos_ris, const QRectF &area)
{
    int index = lengthStroke() - 1;
    for(; index >= 0; index --){
        const stroke &stroke = this->atStroke(index);
        if(unlikely(is_inside_squade(stroke.getBiggerPointInStroke(), area))){
            this->drawStroke(stroke, m_pos_ris);
        }
    }
}

void page::decreseAlfa(const QVector<int> &pos, int decrese)
{
    QPainter painter;
    bool needInit = initImg(false);

    if(unlikely(needInit)){
        WDebug(debugPage, "Warning: page not draw");
        return this->triggerRenderImage(-1, true);
    }

    painter.begin(&this->imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    this->decreseAlfa(pos, &painter, decrese);
    painter.end();
}

QRect page::get_size_area(const QList<stroke> &item, int from, int to)
{
    QRect result;

    if(unlikely(from >= to)){
        return QRect();
    }

    result = item.at(from).getBiggerPointInStroke();

    for(; from < to; from ++){
        const QRect tmp = item.at(from).getBiggerPointInStroke();
        result = datastruct::get_bigger_rect(result, tmp);
    }

    return result;
}

QRect page::get_size_area(const QVector<int> &pos) const
{
    QRect result;
    int len = pos.length();
    QRect tmp;

    if(unlikely(!len)){
        return QRect();
    }

    len --;
    result = atStroke(pos.first()).getBiggerPointInStroke();

    for(; len >= 0; len --){
        tmp = atStroke(pos.at(len)).getBiggerPointInStroke();
        result = datastruct::get_bigger_rect(result, tmp);
    }

    return result;
}

void page::drawForceColorStroke(const stroke &stroke, int m_pos_ris, const QColor &color, QPainter *painter)
{
    Define_PEN(pen);
    cbool needDelete = (bool) (!painter);

    if(needDelete){
        if(unlikely(initImg(false)))
            return this->triggerRenderImage(m_pos_ris, true);

        painter = new QPainter;

        painter->begin(&this->imgDraw);
        painter->setRenderHint(QPainter::Antialiasing, true);
    }

    this->drawStroke(*painter, stroke, pen, color);

    if(needDelete){
        painter->end();
        delete painter;
    }
}

void page::drawForceColorStroke(const QVector<int> &pos, int m_pos_ris, const QColor &color)
{
    QPainter painter;

    if(initImg(false))
        return this->triggerRenderImage(m_pos_ris, true);

    painter.begin(&this->imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(const auto &index : pos){
        const stroke &stroke = atStroke(index);
        this->drawForceColorStroke(stroke, m_pos_ris, color, &painter);
    }
    painter.end();
}

void page::allocateStroke(int numAllocation)
{
    for(int i = 0; i < numAllocation; i++){
        this->m_stroke.append(stroke());
    }
}

#define DO_CTRL(function) \
    err = function; \
    if(err != OK) \
        return err;

int page::save(zip_source_t *file) const
{
    int i, err = OK;
    int len = lengthStroke();

    for(i = 0; i < len; i++){
        DO_CTRL(atStroke(i).save(file));
    }

    for(i = 0; i < 2; i++){
        DO_CTRL(m_stroke_writernote[i].save(file));
    }

    return OK;
}

#define DO_LOAD(list)                               \
    for(i = 0; i < len_stroke; i++){                \
        stroke __tmp;                               \
        DO_CTRL(__tmp.load(file, ver_stroke));      \
        list.append(__tmp);                         \
    }

int page::load(zip_file_t *file, int ver_stroke, int len_stroke)
{
    int i, err;

    DO_LOAD(m_stroke);

    if(ver_stroke == 0){
#ifdef ALL_VERSION
        QList<stroke> tmp;
        SOURCE_READ_RETURN(file, &len_stroke, sizeof(int));

        DO_LOAD(tmp);
        for(int i = lengthStroke() - 1; i >= 0; i--){
            if(atStroke(i).length() == 0){
                this->m_stroke.removeAt(i);
            }
        }
#else
        return ERROR;
#endif
    }
    if(ver_stroke == 1){
        m_stroke_writernote[0].load(file, ver_stroke);
        m_stroke_writernote[1].load(file, ver_stroke);
    }

    return OK;
}

void page::drawStroke(const stroke &stroke, int m_pos_ris)
{
    drawForceColorStroke(stroke, m_pos_ris, stroke.getColor(1.0), NULL);
}
