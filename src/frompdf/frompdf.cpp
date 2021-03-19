#include "frompdf.h"
#include <QPdfium>
#include "../utils/dialog_critic/dialog_critic.h"
#include "../currenttitle/currenttitle_class.h"
#include "../utils/permission/permission.h"

frompdf::frompdf(currenttitle_class *data)
{
    m_data = data;
}

bool frompdf::isvalid(QString &pos){
    QPdfium pdf(pos);
    return pdf.isValid();
}

void frompdf::add(QString &pos)
{
    if(permission::open(pos.toUtf8().constData(), permission::readOnly)){
        return dialog_critic("The file is not readable");
    }

    if(!isvalid(pos)){
        return dialog_critic("The pdf is not valid");
    }

    QPdfium pdf(pos);
    int i, len;
    QImage temp;

    for(i=0, len = pdf.pageCount(); i<len; i++){
        temp = pdf.page(i).image();
        if(temp.isNull()){
            return dialog_critic("I had a problem reading page " + QString::number(i));
        }
    }


}
