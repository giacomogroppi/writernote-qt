#include "fast_sheet_ui.h"
#include "ui_fast_sheet_ui.h"

#include "../../utils/setting_define.h"
#include <QSettings>
#include <QAction>
#include "../../utils/dialog_critic/dialog_critic.h"

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

    this->load();

}

fast_sheet_ui::~fast_sheet_ui()
{
    int i;
    QList<QListWidgetItem *> item;

    this->save();

    for(i=0; i<ui->list_sheet->count(); ++i)
        item.append(ui->list_sheet->item(i));
    for(i=0; i<item.length(); ++i)
        delete item.at(i);

    delete ui;
}

void fast_sheet_ui::needToReload()
{
    this->load();
}

void fast_sheet_ui::load(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    QListWidgetItem * action;
    uint i, len;

    setting.beginGroup(GROUPNAME_AUTO_CREATE_SHEET);

    this->auto_create = setting.value(KEY_AUTO_CREATE_SHEET, true).toBool();

    this->ui->autocreate_sheet->setChecked(this->auto_create);
    emit changeButton(!this->auto_create);


    len = ui->list_sheet->count();

    for(i=len; i < m_style.length(); ++i){
        action = new QListWidgetItem;
        ui->list_sheet->addItem(action);
    }

    for(i=len; i > m_style.length(); --i){
        ui->list_sheet->item(i)->setHidden(true);
    }

    for(i=0; i < m_style.length(); i++){
        const QString &ref = m_style.at(i)->nome;
        ui->list_sheet->item(i)->setText(ref);
    }

    this->m_how = static_cast<n_style>(setting.value(KEY_AUTO_CREATE_STYLE_FAST, n_style::empty).toInt());
    if(m_how == n_style::empty){
        m_how = n_style::square;
    }
    updateCheck();

    this->on_autocreate_sheet_clicked();
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

    const QString &tmp = item->text();
    uint in;
    bool find = false;

    for(in=0; in < m_style.length(); in++){
        if((QString)m_style.at(in)->nome == tmp){
            find = true;
            break;
        }
    }

    if(find)
        this->m_style.saveDefault(in);
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
