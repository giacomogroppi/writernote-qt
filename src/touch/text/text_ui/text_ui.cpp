#include "text_ui.h"
#include "ui_text_ui.h"

#include <QDebug>
#include "../../../utils/color/color_chooser.h"
#include <QSettings>
#include "../../../utils/setting_define.h"
#include "../../../utils/remove_key/remove_key.h"
#include "../../../utils/color/setcolor.h"

text_ui::text_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::text_ui)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    this->loadData();
}

text_ui::~text_ui()
{
    delete ui;
}

static void setBlack(colore_s *color){
    for(int i=0; i<3; i++)
        color->colore[i] = 0;
    color->colore[3] = 255;
}

void text_ui::loadData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_TEXT_PEN);

    QByteArray array;
    colore_s temp;
    setBlack(&temp);

    array.append((const char *)&temp);

    array = setting.value(KEY_TEXT_PEN_COLOR, array).toByteArray();

    if(array.size() != sizeof(temp)){
        /* error */

        /* the function automatically
         * opens and closes the group.
         * to do this we must first close
         * it. to synchronize data
        */
        setting.endGroup();

        remove_key(KEY_TEXT_PEN_COLOR, GROUPNAME_TEXT_PEN);

        /* we need to open it */
        setting.beginGroup(GROUPNAME_TEXT_PEN);

        memcpy(&this->m_currentDcolor, &temp, sizeof(temp));
    }
    else{
        colore_s *temp_;
        temp_ = (colore_s *)array.constData();

        memcpy(&this->m_currentDcolor, temp_, sizeof(temp));
    }

    setting.value(KEY_TEXT_PEN_SIZE, this->m_currentSize).toInt();

    QString temp_font = setting.value(KEY_TEXT_PEN_FONT, "Times").toString();

    this->m_font = QFont(temp_font);

    setting.endGroup();
}

void text_ui::saveData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_TEXT_PEN);

    setting.setValue(KEY_TEXT_PEN_FONT, this->m_font.toString());
    setting.setValue(KEY_TEXT_PEN_FONT, this->m_currentSize);

    QByteArray array;
    array.append((const char *)&this->m_currentDcolor, sizeof(this->m_currentDcolor));

    setting.setValue(KEY_TEXT_PEN_COLOR, array);

    setting.endGroup();
}

bool text_ui::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();


    return QWidget::event(event);
}


void text_ui::on_button_color_clicked()
{
    QColor colorTemp = color_chooser();
    if(!colorTemp.isValid())
        return;


    setcolor_struct(&this->m_currentDcolor, colorTemp);

}

void text_ui::drawButton(){
    ui->button_color->setAutoFillBackground(true);
    QPalette pal = ui->button_color->palette();

    pal.setColor(QPalette::Button, setcolor(&this->m_currentDcolor));
    ui->button_color->setPalette(pal);

    ui->button_color->update();
}

void text_ui::on_spinBox_valueChanged(int arg1)
{
    this->m_currentSize = arg1;
}

void text_ui::on_fontComboBox_currentFontChanged(const QFont &f)
{
    this->m_font = f;
}
