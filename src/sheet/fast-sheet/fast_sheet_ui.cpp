#include "fast_sheet_ui.h"
#include "ui_fast_sheet_ui.h"

#include "../../utils/setting_define.h"
#include <QSettings>
#include <QAction>

fast_sheet_ui::fast_sheet_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fast_sheet_ui)
{
    ui->setupUi(this);

    this->load();

    ui->white_sheet->setCheckable(true);
    ui->lines_sheet->setCheckable(true);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

}

fast_sheet_ui::~fast_sheet_ui()
{
    if(this->m_style)
        delete m_style;

    delete ui;
}

void fast_sheet_ui::needToReload()
{
    if(this->m_style)
        delete this->m_style;

    this->load();
}

void fast_sheet_ui::load(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);

    setting.beginGroup(GROUPNAME_AUTO_CREATE_SHEET);

    this->auto_create = setting.value(KEY_AUTO_CREATE_SHEET, true).toBool();

    this->ui->autocreate_sheet->setChecked(this->auto_create);

    /* load style */
    auto temp = load_last_style();

    if(temp){
        this->m_style = new style_struct;
        memcpy(this->m_style, temp, sizeof(*this->m_style));

    }else{
        this->m_style = nullptr;
        ui->list_sheet->reset();
        return;
    }

    QAction * action;

    for(int i=0; i<QUANTESTRUCT && i < m_style->quanti; i++){
        action = new QAction(this);

        action->setText(m_style->style[i].nome);

        ui->list_sheet->addAction(action);
    }

    int res = setting.value(KEY_AUTO_CREATE_STYLE_FAST, n_style::empty).toInt();

    this->m_how = static_cast<n_style>(res);

    ui->white_sheet->setChecked(m_how == n_style::white);
    ui->lines_sheet->setChecked(m_how == n_style::line);
    ui->shared_sheet->setChecked(m_how == n_style::square);
}

bool fast_sheet_ui::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

void fast_sheet_ui::on_white_sheet_clicked()
{

}
