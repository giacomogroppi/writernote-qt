#include "testingcore.h"
#include "utils/WCommonScript.h"
#include "datawrite/savefile.h"
#include "dataread/xmlstruct.h"
#include "dataread/readlistarray.h"
#include <time.h>
#include <stdlib.h>

#if (defined(DEBUG_CORE) || defined(DEBUGINFO)) && defined(CLOUD)
TestingCore::TestingCore(MainWindow *__parent)
{
    srand(time(NULL));
    parent = __parent;
}

static int num(int from, int to)
{
    return rand() % to + from;
}

int TestingCore::startTesting()
{
    return 0;
}

int TestingCore::createPage(
        Document &doc, int page,
        int lenStroke, int lenPerStroke,
        bool is_same_pressure,
        const PointF &point,
        const double deltaPerPoint,
        const double deltaPerStroke,
        int last_id)
{
    int i, k;
    double x, y;
    datastruct *data = doc.datatouch;
    double base_x, base_y;

    last_id ++;

    for(i = 0; i < lenStroke; i++){
        stroke __tmp;

        base_x = double(i) * deltaPerStroke;
        base_y = double(i) * deltaPerStroke;

        x = point.x();
        y = point.y();

        for(k = 0; k < lenPerStroke; k++){
            Point point;
            point._x = x + deltaPerPoint + base_x;
            point._y = y + deltaPerPoint + page::getHeight() + base_y;

            x += deltaPerPoint;
            y += deltaPerPoint + double(page * page::getHeight());



            __tmp.append(point, stroke_append_default);
        }

        __tmp.setColor(Qt::black);

        data->at_mod(page).append(__tmp);
    }
    return last_id;
}

#define T_PAGE_LEN 10
#define T_STROKE_IN_PAGE 1000
#define T_POINT_IN_STROKE 300
void TestingCore::createDocument(Document *doc)
{
    int i, id = 0;
    if(!doc){
        doc = parent->_canvas->data;
    }

    for(i = 0; i < T_PAGE_LEN; i++){
        page page(i+1, n_style::square);
        doc->datatouch->_page.append(page);
        id = this->createPage(*doc, i, T_STROKE_IN_PAGE, T_POINT_IN_STROKE, false, PointF(0, 0), 0.1, 1, id);

        doc->datatouch->_page.operator[](i).triggerRenderImage(-1, true);
    }

    for(i = 0; i < T_PAGE_LEN; i++){
        qDebug() << doc->datatouch->at(i).lengthStroke();
    }
}

int TestingCore::createAndSave(
        const WString   &pos,
        Document        *doc)
{
    if(!doc)
        doc = parent->_canvas->data;
    createDocument(doc);

    savefile __tmp(pos, doc);
    return __tmp.savefile_check_file(true);
}

int TestingCore::createFile(const WByteArray &folder)
{
    QFile file(folder + "_tmp_write_test.txt");
    constexpr int len = (1024) * 4 * 128;
    int i, res;

    if(un(!file.open(QIODevice::WriteOnly))){
        qDebug() << "Impossibile start testing, file not open";
        return 1;
    }
    for(i = 0; i < len; i += sizeof(res)){
        res = num(0, len);
        file.write((const char *)&res, sizeof(res));
    }
    file.close();

    return 0;
}

int TestingCore::startTest()
{
    WByteArray path_test = "";
    if(createFile(path_test)){
        return 1;
    }

}

#endif
