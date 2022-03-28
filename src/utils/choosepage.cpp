#include "choosepage.h"
#include "ui_choosepage.h"
#include "currenttitle/document.h"
#include "mainwindow.h"
#include <QEvent>

ChoosePage::ChoosePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoosePage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}

ChoosePage::~ChoosePage()
{
    delete ui;
}

void ChoosePage::Show(const Document &doc)
{
    int max = doc.datatouch->lengthPage();
    int curr = doc.datatouch->getFirstPageVisible() + 1;

    this->ui->label_of->setText(qstr(" of %1").arg(max));
    this->ui->plainTextEdit->setPlainText(QString::number(curr));
    this->show();
}

void ChoosePage::on_pushButton_apply_clicked()
{
    MainWindow *main = (MainWindow *)this->parent();
    if(_curr != -1){
        main->_canvas->data->datatouch->moveToPage(_curr - 1);
        main->_canvas->call_update();
    }
}

bool ChoosePage::event(QEvent *event)
{
    constexpr bool eventChoose = true;
    constexpr auto class_func = "ChoosePage::event";

    WDebug(eventChoose, class_func);

    if(unlikely(event->type() == QEvent::WindowDeactivate)){
        this->hide();
        this->_curr = -1;
    }

    return QWidget::event(event);
}


void ChoosePage::on_pushButton_close_clicked()
{
    this->_curr = -1;
    this->close();
}

static void adujstString(QString &str)
{
    int i, len;
    const auto arr = str.toUtf8();
    len = arr.length();

    for(i = 0; i < len; i++){
        const char c = arr.at(i);
        if(c < '0' || c > '9'){
            str.remove(i, 1);
            len --;
            i --;
        }
    }
}

void ChoosePage::on_plainTextEdit_textChanged()
{
    MainWindow *main = (MainWindow *)this->parent();
    bool ok, needToClose = false;
    auto str = ui->plainTextEdit->toPlainText();
    int maxPage = main->_canvas->data->datatouch->lengthPage();

    if(str.indexOf('\n') != -1){
        needToClose = true;
    }

    adujstString(str);
    const int pageSelect = str.toInt(&ok);

    if(!ok){
        if(ui->plainTextEdit->toPlainText() == "")
            return;

        ui->plainTextEdit->clear();
        return;
    }

    if(pageSelect < 1){
        ui->plainTextEdit->setPlainText("1");
        _curr = 0;
        return;
    }

    if(pageSelect > maxPage){
        _curr = maxPage;
        ui->plainTextEdit->setPlainText(QString::number(_curr));
        return;
    }

    _curr = pageSelect;
    if(needToClose){
        this->on_pushButton_apply_clicked();
        this->close();
    }
}


void ChoosePage::on_pushButton_down_clicked()
{
    MainWindow *main = (MainWindow *)parent();
    int maxPage = main->_canvas->data->datatouch->lengthPage();

    if(this->_curr >= 0 && _curr + 1 <= maxPage ){
        _curr ++;
        ui->plainTextEdit->setPlainText(QString::number(_curr));
        this->on_pushButton_apply_clicked();
    }
}


void ChoosePage::on_pushButton_up_clicked()
{
    if(_curr >= 2){
        _curr --;
        ui->plainTextEdit->setPlainText(QString::number(_curr));
        this->on_pushButton_apply_clicked();
    }
}

