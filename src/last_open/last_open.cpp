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

static void tidyup(QList<last_file> &ref,
                   const int m_quanti,
                   struct option_last_open_ui::__r * data);

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

#ifdef ANDROID
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

    delete ui;
}

void last_open::setDataReturn(char **data){
    m_style_return = data;
}

void last_open::updateList(){
    int i, len;
    len = m_lista.length();
    ui->listWidget->setIconSize(QSize(200, 200));

    for(i=0; i<len; i++){
        //ui->listWidget->addItem(m_item.operator[](i));
        //ui->listWidget->setItemWidget(m_item.operator[](i), m_lista.operator[](i));
        //ui->verticalLayout->addWidget(m_lista.at(i));
        QIcon icon(":image/images/not_define.png");

        QListWidgetItem *item = new QListWidgetItem;
        item->setText("/home/tmp/writernote.writer\nYou");
        item->setIcon(icon);

        ui->listWidget->addItem(item);
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
        if(data.val <= 0){
            __val = 255;
        }
        else{
            __val = data.pos;
        }
    }

    const bool ok = load_data(m_quanti, this->m_last);

    if(data.val == option_last_open_ui::open_last){
        on_click_ex(m_last.first().posizione);

        return 0;
    }

    element_ui *temp_element_ui;
    QListWidgetItem *item;

    if(!ok){
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

    tidyup(m_last, m_quanti, &data);

    for(i=0; (i<m_quanti) && (i < __val); i++){
        temp_element_ui = new element_ui;
        item = new QListWidgetItem("");

        temp_element_ui->setData(&m_last[i], i);

        connect(temp_element_ui, SIGNAL(on_pressed(int)), this, SLOT(on_clicked(int)));
        connect(temp_element_ui, SIGNAL(deleteIn(int)), this, SLOT(deleteInElement(int)));

#ifdef CLOUD
        connect(temp_element_ui, SIGNAL(downloadIn(int)), this, SLOT(downloadIn(int)));
#endif // CLOUD

        m_lista.append(temp_element_ui);
        m_item.append(item);
    }


    this->updateList();

    return m_quanti;
}

void last_open::deleteIn(int index){
    int i;
    for(i=index; i<m_quanti-1; i++){
        memcpy(&m_last[i], &m_last[i+1], sizeof(last_file));

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

static void dochange(last_file &first, last_file &second){
    last_file tmp;

    memcpy(&tmp, &first, sizeof(tmp));
    memcpy(&first, &second, sizeof(tmp));
    memcpy(&second, &tmp, sizeof(tmp));
}

/*
 * la funzione riordina i file dal
 * più recente al più vecchio
 * In caso ci siano più copybook
 * del numero settato dall'utente,
 * li alimina automaticamente
 *
 * the function reorders files
 * from newest to oldest
*/
#define REMOVE_CHAR(str, str2, c) str.remove(c); \
    str2.remove(c)

static void tidyup(QList<last_file> &ref,
                   const int m_quanti,
                   option_last_open_ui::__r *data){


    Q_UNUSED(data);

    int i, k;
    QString first, second;

    for(i=0; i<m_quanti-1; ++i){
        for(k=i+1; k<m_quanti; ++k){
            first = ref.at(i).last_modification_g + (QString)ref.at(i).last_modification_o;
            second = ref.at(k).last_modification_g + (QString)ref.at(i).last_modification_o;

            REMOVE_CHAR(first, second, ':');

            if(atoi(first.toUtf8().constData()) < atoi(second.toUtf8().constData())){
                dochange(ref.operator[](i), ref.operator[](k));
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
