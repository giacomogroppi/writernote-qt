#include "ui_restore.h"
#include "ui_ui_restore.h"
#include "../../utils/get_file_dir/get_file_dir.h"

ui_restore::ui_restore(QWidget *parent, QString path) :
    QDialog(parent),
    ui(new Ui::ui_restore)
{
    ui->setupUi(this);

    this->path = path;

    if(path != ""){
        __l = get_file_dir::get(path);
    }

}

ui_restore::~ui_restore()
{
    delete ui;
}
