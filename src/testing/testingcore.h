#pragma once


#include "core/WString.h"
#include "core/PointF.h"
#include "core/WByteArray.h"
#include "currenttitle/document.h"

#if defined(DEBUG_CORE) || defined(DEBUGINFO)

class TestingCore
{
private:
    class MainWindow *parent;
    int createPage(Document & doc, int page, int lenStroke, int lenPerStroke,
                    bool is_same_pressure, const PointF &point,
                    const double deltaPerPoint, const double deltaPerStroke, int last_id);
public:
    TestingCore(class MainWindow *parent);
    int startTesting();
    void createDocument(Document *doc);
    int createAndSave(const WString &pos, Document *doc);
    int createFile(const WByteArray &folder);
    int startTest();
};

#endif

