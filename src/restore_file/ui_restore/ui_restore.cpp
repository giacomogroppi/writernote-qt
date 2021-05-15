#include "ui_restore.h"
#include "ui_ui_restore.h"
#include "../../utils/get_file_dir/get_file_dir.h"
#include "../../utils/common_def.h"
#include <QDir>
#include <QFileDialog>

ui_restore::ui_restore(QWidget *parent, QString path) :
    QDialog(parent),
    ui(new Ui::ui_restore)
{
    ui->setupUi(this);

    m_curr = new currenttitle_class;
    m_ind = new indice_class;

    this->path = path;

    ui->plainTextEdit->setPlainText(path);

    if(path != ""){
        __l = get_file_dir::get(path);
    }

}

ui_restore::~ui_restore()
{

    delete m_curr;
    delete m_ind;
    delete m_save;
    delete m_xml;

    delete ui;
}

void ui_restore::on_close_button_clicked()
{
    this->close();
}

void ui_restore::on_ok_restore_clicked()
{

}

void ui_restore::on_pushButton_open_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if(dir == ""){
        return;
    }

    __l = get_file_dir::get(path);

    updateList();

}

void ui_restore::removeNotWriternote(QStringList &l)
{
    uint i, len;
    len = l.length();

    for(i=0; i<len; ++i){
        if(l.at(i).indexOf(APP_EXT) == -1){
            l.removeAt(i);
            --len;
            --i;
        }
    }

}

void ui_restore::updateList()
{
    uint i, len;

    len = __l.length();
    ui->listWidget->clear();

    for(i=0; i<len; ++i){
        ui->listWidget->addItem(__l.at(i));
    }

}
