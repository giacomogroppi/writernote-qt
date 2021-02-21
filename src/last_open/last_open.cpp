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

static void copy(last_file *s, char *pos = NULL, char *last_mod = NULL, int type = TYPE_CLOUD);

last_open::last_open(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);
}

last_open::~last_open()
{
    int i, len;

    len = m_lista.length();
    for(i=0; i<len; i++){
        delete m_lista.at(i);
    }

    delete ui;
}

void last_open::setDataReturn(last_file **data){
    m_style_return = data;
}

void last_open::updateList(){
    int i, len;
    len = m_lista.length();

    for(i=0; i<len; i++){
        ui->verticalLayout->addWidget(m_lista.at(i));
    }
};

/*
 * the function is call by main
*/
int last_open::load_data_()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    m_quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();
    if(m_quanti == 0)
        return 0;

    int i;

    element_ui *temp_element_ui;

    m_last = load_data(setting, m_quanti);

    for(i=0; i<m_quanti; i++){
        temp_element_ui = new element_ui;

        temp_element_ui->setData(&m_last[i], i);

        connect(temp_element_ui, SIGNAL(on_pressed(int)), this, SLOT(on_clicked(int)));
        connect(temp_element_ui, SIGNAL(deleteIn(int)), this, SLOT(deleteInElement(int)));

#ifdef CLOUD
        connect(temp_element_ui, SIGNAL(downloadIn(int)), this, SLOT(downloadIn(int)));
#endif // CLOUD

        /*
         * void deleteIn(int);
         * void downloadIn(int);
        */

        m_lista.append(temp_element_ui);
    }

    setting.endGroup();

    this->updateList();

    return m_quanti;
}

void last_open::deleteIn(int index){
    int i;
    for(i=index; i<m_quanti; i++){
        copy(&m_last[i], m_last[i].posizione, m_last[i].last_modification, m_last[i].type);

        m_lista.at(i)->decrease();
    }
    m_quanti --;

    delete m_lista.at(index);
    this->m_lista.removeAt(index);

    if(m_quanti == 0){
        *m_style_return = NULL;
        this->close();
    }
}

void last_open::on_clicked(int index)
{
    QFile file((QString)m_last[index].posizione);

    if(file.exists()){
        *m_style_return = new last_file;
        copy(*m_style_return, m_last[index].posizione, NULL, m_last[index].type);

        this->close();
    }
    else{
        dialog_critic("The file didn't exist");

        this->deleteInElement(index);
    }
}

static void copy(last_file *s, char *pos, char *last_mod, int type){
    memcpy(&s->type, &type, sizeof(int));

    if(pos)
        memcpy(&s->posizione, pos, sizeof(char)*MAXSTR_);

    if(last_mod)
        memcpy(&s->last_modification, last_mod, sizeof(char)*MAXMOD_);
}

void last_open::on_open_button_clicked()
{
    QString path;
    qfilechoose chooser(nullptr);
    if(!chooser.filechoose(&path, TYPEFILEWRITER))
        return;

    QFile file(path);

    if(!file.exists()){
        return dialog_critic("The file didn't exist");
    }

    *m_style_return = new last_file;

    strcpy((*m_style_return)->posizione, path.toUtf8().constData());

    this->close();

}

void last_open::deleteInElement(int index){
    int i, len;
    len = m_lista.length();
    for(i=0; i<len; i++){
        ui->verticalLayout->removeWidget(m_lista.at(i));
    }

    deleteIn(index);

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    save_data_f(setting, m_quanti, this->m_last);

    this->updateList();
}

void last_open::downloadIn(int index){
#ifdef CLOUD
    downloadfile(nullptr, "", "");
#else
    dialog_critic("Your version of writernote was not\ncompiled without the cloud package");
#endif
}
