#pragma once


#include <QString>
#include <QPointF>
#include <QByteArray>
#include "currenttitle/document.h"

#if defined(DEBUG_CORE) || defined(DEBUGINFO)

class TestingCore
{
private:
    class MainWindow *parent;
    int createPage(Document & doc, int page, int lenStroke, int lenPerStroke,
                    bool is_same_pressure, const QPointF &point,
                    const double deltaPerPoint, const double deltaPerStroke, int last_id);
public:
    TestingCore(class MainWindow *parent);
    int startTesting();
    void createDocument(Document *doc);
    int createAndSave(const QString &pos, Document *doc);
    int createFile(const QByteArray &folder);
    int startTest();
};

#endif

