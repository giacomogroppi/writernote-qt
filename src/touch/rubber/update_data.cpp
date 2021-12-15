#include "rubber_ui.h"
#include "ui_rubber_ui.h"

void rubber_ui::update_data(){
    ui->size_slider->setValue(m_size_gomma);

    const bool m_type = (this->m_type_gomma == e_type_rubber::total);

    ui->totale_button->setChecked(m_type);
    ui->partial_button->setChecked(!m_type);

    ui->size_slider->setEnabled(true);

}
