#include "fast_sheet_ui.h"
#include "ui_fast_sheet_ui.h"

#include "../../utils/setting_define.h"
#include <QSettings>
#include <QAction>
#include "../../utils/dialog_critic/dialog_critic.h"
#include "../load_last_style.h"

static void uncheck(QListWidget *list, fast_sheet_ui::n_style temp){
    if(temp != fast_sheet_ui::empty)
        list->unsetCursor();

    //list->currentItem()->setCheckState(Qt::CheckState::Unchecked);
}

fast_sheet_ui::fast_sheet_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fast_sheet_ui)
{
    ui->setupUi(this);

    ui->white_sheet->setCheckable(true);
    ui->lines_sheet->setCheckable(true);
    ui->shared_sheet->setCheckable(true);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    qDebug() << "fast_sheet_ui: 1";
    this->load();
    qDebug() << "fast_sheet_ui: 2";
}

fast_sheet_ui::~fast_sheet_ui()
{
    if(this->m_style)
        delete m_style;

    this->save();

    delete ui;
}

void fast_sheet_ui::needToReload()
{
    if(this->m_style)
        delete this->m_style;

    this->m_style = nullptr;

    this->load();
}

void fast_sheet_ui::load(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    QAction * action;
    uint i, len;
    const style_struct *tmp;

    setting.beginGroup(GROUPNAME_AUTO_CREATE_SHEET);
    qDebug() << "fast_sheet_ui::load() 1";
    this->auto_create = setting.value(KEY_AUTO_CREATE_SHEET, true).toBool();

    this->ui->autocreate_sheet->setChecked(this->auto_create);
    emit changeButton(!this->auto_create);
    qDebug() << "fast_sheet_ui::load() 2";
    /* load style */
    tmp = load_last_style();

    if(tmp){
        this->m_style = new style_struct;
        memcpy(this->m_style, tmp, sizeof(*this->m_style));
        qDebug() << "fast_sheet_ui::load() 3";
    }else{
        qDebug() << "fast_sheet_ui::load() 4";
        this->m_style = nullptr;
        ui->list_sheet->reset();
        this->on_autocreate_sheet_clicked();
        goto cont_;
    }
    qDebug() << "fast_sheet_ui::load() 5";
    len = ui->list_sheet->count();

    /*
     * we want to add the item first, and in the new step edit
     * when this works is executed the first time, the number
     * of objects in the list is zero, but since we want to be
     *  able to execute it later, and we don't want to delete
     *  the list every time and create new objects,
     *  we dynamically add and hidden objects from the list
    */
    for(i=len; i < (uint)m_style->quanti && i<QUANTESTRUCT; ++i){
        action = new QAction(this);
        ui->list_sheet->addAction(action);
    }
    qDebug() << "fast_sheet_ui::load() 6";
    for(i=len; i > (uint)m_style->quanti; --i){
        ui->list_sheet->item(i)->setHidden(true);
    }
    qDebug() << "fast_sheet_ui::load() 7";
    for(i=0; i<QUANTESTRUCT && i < (uint)m_style->quanti; i++){
        ui->list_sheet->item(i)->setText(m_style->style[i].nome);
    }
    qDebug() << "fast_sheet_ui::load() 8";
    cont_:

    this->m_how = static_cast<n_style>(setting.value(KEY_AUTO_CREATE_STYLE_FAST, n_style::empty).toInt());
    if(m_how == n_style::empty){
        m_how = n_style::square;
    }
    qDebug() << "fast_sheet_ui::load() 9";
    updateCheck();

    this->on_autocreate_sheet_clicked();
    qDebug() << "fast_sheet_ui::load() 10";
    setting.endGroup();
}

bool fast_sheet_ui::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

void fast_sheet_ui::on_white_sheet_clicked()
{
    m_how = fast_sheet_ui::white;

    updateCheck();
}


void fast_sheet_ui::on_lines_sheet_clicked()
{
    m_how = fast_sheet_ui::line;

    updateCheck();
}

void fast_sheet_ui::on_shared_sheet_clicked()
{
    m_how = fast_sheet_ui::square;

    updateCheck();
}

void fast_sheet_ui::on_list_sheet_itemClicked(QListWidgetItem *item)
{
    m_how = fast_sheet_ui::empty;

    updateCheck();

    QString temp = item->text();
    int in = 0;
    bool find = false;

    for(in=0; in < m_style->quanti; in++){
        if(m_style->style[in].nome == temp){
            find = true;
            break;
        }
    }

    if(find)
        save_default_drawing(&in);
    else
        dialog_critic("We had a big problem figuring out what style you clicked");
}

void fast_sheet_ui::on_autocreate_sheet_clicked()
{
    this->auto_create = ui->autocreate_sheet->isChecked();
    emit changeButton(auto_create);
}

void fast_sheet_ui::save(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUTO_CREATE_SHEET);

    setting.setValue(KEY_AUTO_CREATE_SHEET, this->auto_create);

    setting.setValue(KEY_AUTO_CREATE_STYLE_FAST, m_how);

    setting.endGroup();
}

void fast_sheet_ui::updateCheck()
{
    ui->white_sheet->setChecked(m_how == n_style::white);
    ui->lines_sheet->setChecked(m_how == n_style::line);
    ui->shared_sheet->setChecked(m_how == n_style::square);
    uncheck(ui->list_sheet, m_how);
}
