#include "extract_audio_ui.h"
#include "ui_extract_audio_ui.h"

extract_audio_ui::extract_audio_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::extract_audio_ui)
{
    ui->setupUi(this);
}

extract_audio_ui::~extract_audio_ui()
{
    delete ui;
}
