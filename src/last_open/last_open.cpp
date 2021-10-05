#include "last_open.h"
#include "ui_last_open.h"
#include <QSettings>
#include "../utils/setting_define.h"
#include <QAction>
#include <QFile>
#include <QFileInfo>
#include "../utils/dialog_critic/dialog_critic.h"
#include "../datawrite/qfilechoose.h"
#include "string.h"
#include "../cloud/utils/downloadfile.h"
#include "../utils/remove_key/remove_key.h"
#include "../datawrite/qfilechoose.h"
#include "option/option_last_open_ui.h"
#include "widget_parent/widget_parent.h"
#include "../utils/get_path_application.h"

last_open::last_open(QWidget *parent,
                     struct struct_user *user,
                     cloud_controll *controll,
                     bool *close_all,
                     Method method) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);
    m_user = user;
    m_controll = controll;
    m_closeall = close_all;
    m_currentMethod = method;

#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    this->setWindowState(Qt::WindowFullScreen);
    this->ui->open_button->setHidden(true);
#endif

    ui->option_button->setHidden(m_currentMethod == Method::OpenFile);

    if(!m_closeall){
        this->ui->close_all->hide();
    }

    this->loadGeometry();
}

last_open::~last_open()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY_LAST_OPEN_FILE);
    setting.setValue(KEY_GEOMETRY_LAST_OPEN_FILE, this->geometry());
    setting.endGroup();

    if(m_parent)
        delete m_parent;
    delete ui;
}

void last_open::setDataReturn(char **data){
    m_style_return = data;
}

void last_open::updateList(){
    m_parent->updateList();
};


/*
 * the function is call by main.cpp
*/
int last_open::load_data_()
{
    int __val;
    uint i;
    option_last_open_ui option(nullptr);
    bool ok;

    struct option_last_open_ui::__r data = option.getData();
    if(data.val == option_last_open_ui::disable)
        return 0;
    if(data.val == option_last_open_ui::enable){
        if(data.val <= 0)
            __val = 255;
        else
            __val = data.pos;

    }

    if(m_currentMethod == Method::OpenRecent){
        ok = this->m_last.load_data();
        if(!ok)
            return 0;
        if(data.val == option_last_open_ui::open_last){
            on_click_ex(m_last.at(0).posizione);

            return 0;
        }
    }else if(m_currentMethod == Method::OpenFile){
        ok = this->m_last.load_folder(get_path_application::exe());
        if(!ok)
            return 0;
    }else{
        assert(0);
    }

    /*
     * check that the position of the files is not
     * null, in case it translates all elements
    */

    for(i=0; i<m_last.length(); ++i){
        if(!strlen(m_last.at(i).posizione))
            m_last.removeAt(i);
    }


    this->m_last.tidyup();

    this->m_parent = new widget_parent(nullptr, &m_last, data.showOnlyName, this);
    this->ui->scrollArea->setWidget(m_parent);
    this->updateList();

    return m_last.length();
}

void last_open::loadGeometry()
{
    QRect geo;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY_LAST_OPEN_FILE);
    geo = setting.value(KEY_GEOMETRY_LAST_OPEN_FILE, this->geometry()).toRect();
    this->setGeometry(geo);
    setting.endGroup();
}

void last_open::deleteIn(int index){
    this->m_parent->decrease(index);

    if(this->m_last.length() == 0){
        *m_style_return = NULL;
        this->close();
    }
}

/* return 1 if the file didn't exist */
uchar last_open::on_click_ex(const char *pos){
    const size_t len = strlen(pos);
    if(QFile::exists(pos)){
        *m_style_return = new char[len+1];
        strncpy(*m_style_return, pos, len+1);

        this->close();
        return 0;
    }
    return 1;
}

void last_open::on_clicked(int index)
{
    if(on_click_ex(m_last.at(index).posizione)){
        dialog_critic("The file didn't exist");

        this->deleteInElement(index);
    }
}

void last_open::on_open_button_clicked()
{
    QString path;
    if(!qfilechoose::filechoose(path, TYPEFILEWRITER))
        return;

    QFile file(path);

    if(!file.exists()){
        return dialog_critic("The file didn't exist");
    }

    *m_style_return = new char[path.length()+1];

    strcpy(*m_style_return, path.toUtf8().constData());

    this->close();

}

void last_open::deleteInElement(int index){
    deleteIn(index);

    this->m_last.save_data_setting();
    this->updateList();
}

void last_open::downloadIn(int index){
#ifdef CLOUD
    QString m_to;
    qfilechoose temp(NULL);
    if(!temp.filechoose(&m_to))
        return;

    dowloadfile(m_user, m_last->posizione, m_to);

#else
    Q_UNUSED(index);
    dialog_critic("Your version of writernote was\ncompiled without the cloud support");
#endif
}

void last_open::on_close_all_clicked()
{
    if(m_closeall)
        *m_closeall = true;
    this->close();
}

void last_open::on_close_button_clicked()
{
    this->close();
}

