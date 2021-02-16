#include "last_open.h"
#include "ui_last_open.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include <QAction>

#include <QFile>
#include <QFileInfo>

#include "../dialog_critic.h"

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
    m_last_file = data;
}

void last_open::updateList(){
    int i, len;
    len = m_lista.length();

    for(i=0; i<len; i++){
        ui->verticalLayout->addWidget(m_lista.at(i));
    }
};

int last_open::load_data_()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    m_quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();
    if(m_quanti == 0)
        return 0;

    int i;

    element_ui *temp_element_ui;

    last_file * temp_struct = NULL;

    if(m_quanti)
        temp_struct = load_data(setting);

    for(i=0; i<m_quanti; i++){
        temp_element_ui = new element_ui;

        temp_element_ui->setData(&temp_struct[i], i);

        connect(temp_element_ui, SIGNAL(on_pressed(int)), this, SLOT(on_clicked(int)));

        m_lista.append(temp_element_ui);
    }

    setting.endGroup();

    this->m_last = new last_file[m_quanti];
    memccpy(m_last, temp_struct, m_quanti, sizeof(last_file));


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
}

void last_open::on_clicked(int index)
{
    QFile file((QString)m_last[index].posizione);

    if(file.exists()){
        *m_last_file = new last_file;
        copy(*m_last_file, m_last[index].posizione, NULL, m_last[index].type);

        this->close();
    }
    else{
        dialog_critic("The file didn't exist");

        int i, len;
        len = m_lista.length();
        for(i=0; i<len; i++){
            ui->verticalLayout->removeWidget(m_lista.at(i));
        }

        deleteIn(index);

        this->updateList();
    }
}

static void copy(last_file *s, char *pos, char *last_mod, int type){
    memcpy(&s->type, &type, sizeof(int));

    if(pos)
        memcpy(&s->posizione, pos, sizeof(char)*MAXSTR_);

    if(last_mod)
        memcpy(&s->last_modification, last_mod, sizeof(char)*MAXMOD_);
}
