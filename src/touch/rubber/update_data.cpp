#include "rubber_ui.h"
#include "ui_rubber_ui.h"

void rubber_ui::update_data(){
    ui->size_slider->setValue(m_size_gomma);

    ui->totale_button->setChecked(this->m_type_gomma == TOTALE);
    ui->partial_button->setChecked(this->m_type_gomma == PARZIALE);
}
