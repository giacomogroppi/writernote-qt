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

static void copy(last_file *s,
                 char *pos = NULL,
                 char *last_mod_o = NULL,
                 char *last_mod_g= NULL,
                 int type = TYPE_CLOUD);

static void tidyup(last_file *, int);

last_open::last_open(QWidget *parent,
                     struct struct_user *user,
                     cloud_controll *controll) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);
    m_user = user;
    m_controll = controll;
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

void last_open::setDataReturn(char **data){
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
 * the function is call by main.cpp
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

    if(m_last == NULL){
        remove_key(KEY_LAST_BASE_FILE, GROUPNAME_LAST_FILE);
        remove_key(KEY_LAST_FILE_QUANTI, GROUPNAME_LAST_FILE);

        return 0;
    }

    tidyup(m_last, m_quanti);

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
        copy(&m_last[i], m_last[i].posizione, m_last[i].last_modification_o, m_last[i].last_modification_g, m_last[i].type);

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
        *m_style_return = new char[strlen(m_last[index].posizione)+1];
        strcpy(*m_style_return, m_last[index].posizione);

        //copy(*m_style_return, m_last[index].posizione, NULL, NULL, m_last[index].type);

        this->close();
    }
    else{
        dialog_critic("The file didn't exist");

        this->deleteInElement(index);
    }
}

static void copy(last_file *s, char *pos, char *last_mod_o, char *last_mod_g, int type){
    memcpy(&s->type, &type, sizeof(int));

    if(pos)
        memcpy(&s->posizione, pos, sizeof(char)*MAXSTR_);

    if(last_mod_o)
        memcpy(&s->last_modification_o, last_mod_o, sizeof(char)*MAXMOD_);

    if(last_mod_g)
        memcpy(&s->last_modification_g, last_mod_g, sizeof(char)*MAXMOD_);
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

    *m_style_return = new char[path.length()+1];

    strcpy(*m_style_return, path.toUtf8().constData());

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
    QString m_to;
    qfilechoose temp(NULL);
    if(!temp.filechoose(&m_to))
        return;

    dowloadfile(m_user, m_last->posizione, m_to);

#else
    Q_UNUSED(index);
    dialog_critic("Your version of writernote was not\ncompiled without the cloud package");
#endif
}


#define CARATTERE ':'

static void remove_c(QString *stringa){
    int i,len = stringa->length();
    for(i=0; i<len; i++)
        if(stringa->at(i) == CARATTERE)
            stringa->remove(i);
}

/*
 * return MAX if the first time is after the second
 * return MIN if the second time is after the first
*/
#define SAME 0
#define MIN -1
#define MAX 1
static int recent_private(QString first, QString second){

    remove_c(&first);
    remove_c(&second);

    int m_first, m_second;
    m_first = atoi(first.toUtf8().constData());
    m_second = atoi(first.toUtf8().constData());

    if(m_first == m_second)
        return SAME;
    if(m_first > m_second)
        return MAX;

    return MIN;
}

/*
 * the function return true if we need to change position
*/
static bool recent(last_file *first, last_file *second){
    int res = recent_private((QString)first->last_modification_g, (QString)second->last_modification_g);

    if(res == SAME){
        res = recent_private((QString)first->last_modification_o, (QString)second->last_modification_o);

        if (res == SAME)
            /* if they are the saim time */
            return false;

        if(res == MIN)
            return true;

        return false;
    }

    /*
     * if res == MIN the second time is after [we need to change]
    */
    return res == MIN;

}

/*
 * la funzione riordina i file dal più recente al più vecchio
*/
static void tidyup(last_file *m_data, int m_quanti){
    int k;
    last_file temp;
    for(int i=0; i<m_quanti; i++){
        for(k=1; k<m_quanti; k++){
            if(recent(&m_data[i], &m_data[k])){
                temp = m_data[i];
                m_data[i] = m_data[k];
                m_data[k] = temp;
            }
        }
    }
}

