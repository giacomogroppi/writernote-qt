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

static void copy(last_file *s,
                 char *pos = NULL,
                 char *last_mod_o = NULL,
                 char *last_mod_g= NULL,
                 int type = TYPE_CLOUD);

static void tidyup(last_file *, int);

last_open::last_open(QWidget *parent,
                     struct struct_user *user,
                     cloud_controll *controll,
                     bool *close_all) :
    QDialog(parent),
    ui(new Ui::last_open)
{
    ui->setupUi(this);
    m_user = user;
    m_controll = controll;
    m_closeall = close_all;

#if defined(__ANDROID__)
    this->setWindowState(Qt::WindowFullScreen);
#endif

}

last_open::~last_open()
{
    int i, len;

    len = m_lista.length();
    for(i=0; i<len; i++){
        delete m_lista.at(i);
    }

    if(this->m_last)
        free(this->m_last);

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
    m_quanti = load_quanti();

    if(m_quanti == 0)
        return 0;

    int i, __val;

    option_last_open_ui option(nullptr);

    struct option_last_open_ui::__r data = option.getData();

    if(data.val == option_last_open_ui::disable)
        return 0;

    if(data.val == option_last_open_ui::enable){
        if(data.pos == -1){
            __val = 255;
        }
        else{
            __val = data.pos;
        }
    }

    if(data.val == option_last_open_ui::open_last){

    }

    element_ui *temp_element_ui;

    m_last = load_data(m_quanti);

    if(m_last == NULL){
        remove_key(KEY_LAST_BASE_FILE, GROUPNAME_LAST_FILE);
        remove_key(KEY_LAST_FILE_QUANTI, GROUPNAME_LAST_FILE);

        return 0;
    }

    /*
     * check that the position of the files is not
     * null, in case it translates all elements
    */
    {
        bool check = false;
        for(int i=0, k; i<m_quanti; i++){
            if(strlen(m_last[i].posizione) == 0){
                for(k=i; k<m_quanti-1; k++){
                    memcpy(&m_last[k], &m_last[k+1], sizeof(last_file));
                }
                m_quanti --;

                check = true;
            }
        }

        if(check){
            save_data_f(m_quanti, m_last);
        }
    }

    tidyup(m_last, m_quanti);

    for(i=0; i<m_quanti && i < __val; i++){
        temp_element_ui = new element_ui;

        temp_element_ui->setData(&m_last[i], i);

        connect(temp_element_ui, SIGNAL(on_pressed(int)), this, SLOT(on_clicked(int)));
        connect(temp_element_ui, SIGNAL(deleteIn(int)), this, SLOT(deleteInElement(int)));

#ifdef CLOUD
        connect(temp_element_ui, SIGNAL(downloadIn(int)), this, SLOT(downloadIn(int)));
#endif // CLOUD

        m_lista.append(temp_element_ui);
    }


    this->updateList();

    return m_quanti;
}

void last_open::deleteIn(int index){
    int i;
    for(i=index; i<m_quanti-1; i++){
        memcpy(&m_last[i], &m_last[i+1], sizeof(*m_last));
        //copy(&m_last[i], m_last[i].posizione, m_last[i].last_modification_o, m_last[i].last_modification_g, m_last[i].type);

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

/* return 1 if the file didn't exist */
uchar last_open::on_click_ex(const char *pos){
    if(QFile::exists(pos)){
        *m_style_return = new char[strlen(pos)+1];
        strcpy(*m_style_return, pos);

        this->close();
        return 0;
    }
    return 1;
}

void last_open::on_clicked(int index)
{
    if(on_click_ex(m_last[index].posizione)){
        dialog_critic("The file didn't exist");

        this->deleteInElement(index);
    }
}

static void copy(last_file *s,
                 char *pos,
                 char *last_mod_o,
                 char *last_mod_g,
                 int type){
    memcpy(&s->type, &type, sizeof(int));

    if(pos)
        memcpy(&s->posizione, pos, sizeof(char)*MAXSTR__FILE);

    if(last_mod_o)
        memcpy(&s->last_modification_o, last_mod_o, sizeof(char)*MAXMOD__FILE);

    if(last_mod_g)
        memcpy(&s->last_modification_g, last_mod_g, sizeof(char)*MAXMOD__FILE);
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

    save_data_f(m_quanti, this->m_last);

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


#define CARACTER ':'
static void remove_c(QString *stringa){
    int i,len = stringa->length();
    for(i=0; i<len; i++)
        if(stringa->at(i) == CARACTER)
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
    //if(m_quanti == 1 || m_quanti == 0)
    //    return;

    int k;
    size_t s;
    s = sizeof(last_file);

    last_file temp;

    for(int i=0; i<m_quanti; i++){
        for(k=1; k<m_quanti; k++){
            if(recent(&m_data[i], &m_data[k])){
                memcpy(&temp, &m_data[i], s);
                memcpy(&m_data[i], &m_data[k], s);
                memcpy(&m_data[k], &temp, s);

            }
        }
    }

}


void last_open::on_close_all_clicked()
{
    *m_closeall = true;
    this->close();
}

void last_open::on_close_button_clicked()
{
    this->close();
}
