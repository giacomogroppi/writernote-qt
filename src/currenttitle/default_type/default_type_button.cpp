#include "mainwindow.h"
#include "default_type.h"
#include "utils/default_option/default_option_ui.h"

static default_option_ui * n_def = nullptr;
static default_type::e_type_copybook __res;
static bool __need_save = false;

void MainWindow::on_actionDefault_method_write_triggered()
{
    __res = default_type::load_preference_copybook();

    if(!n_def){
        n_def = new default_option_ui(this,
                                      "Pen",
                                      "Keyboard",
                                      "Insert method",
                                      "With writernote you can write in two way, one with the pen,\n and with the keyboard, choose the default choise or enter cancel",
                                      __res == default_type::e_type_copybook::pen);


        QObject::connect(n_def, &default_option_ui::result, [&](default_option_ui::e_result res){
            if(res == default_option_ui::first){
                __res = default_type::pen;
            }else if(res == default_option_ui::second){
                __res = default_type::key;
            }

            __need_save = true;
        });

    }


    if(__res == default_type::pen){
        n_def->setChecked(default_option_ui::first);
    }else if(__res == default_type::key){
        n_def->setChecked(default_option_ui::second);
    }

    n_def->exec();

    if(__need_save)
        default_type::save_preference_copybook(__res);

    __need_save = false;

}
