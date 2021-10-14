#ifndef OPTION_LAST_OPEN_UI_H
#define OPTION_LAST_OPEN_UI_H

#include <QDialog>

namespace Ui {
class option_last_open_ui;
}

#define option_last_open_ui_def_num 6

class option_last_open_ui : public QDialog
{
    Q_OBJECT

public:
    enum option: int{
        enable,
        disable,
        open_last /* open directly the last file open */
    };

    explicit option_last_open_ui(QWidget *parent = nullptr);
    ~option_last_open_ui();

    struct __r {
        enum option val; /* type */
        int pos = option_last_open_ui_def_num; /* number of lastopen file to show */
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
        const bool showOnlyName = true;
        const bool showFileIfExist = true; /* only if it exist */
#else
        bool showOnlyName = false;
        bool showFileIfExist = false;
#endif

    };

    inline struct __r getData(){
        return data;
    };

    struct __r data;

    void saveData();
    void loadData();

private:
    Ui::option_last_open_ui *ui;

    void updateShow();

signals:

    /*
     *  number = -1 Show all file
    */
    void changeSomething(enum option,
                         char number);

private slots:
    void on_pushButton_enable_clicked();
    void on_pushButton_always_enable_clicked();
    void on_pushButton_disable_clicked();
    void on_ok_button_clicked();
    void on_cancel_button_clicked();


    void on_spinBox_valueChanged(int arg1);
    void on_showOnlyName_stateChanged(int arg1);
    void on_checkBox_stateChanged(int arg1);
};

#endif // OPTION_LAST_OPEN_UI_H
