#ifndef SETTING_RESTORE_UI_H
#define SETTING_RESTORE_UI_H

#include <QDialog>
#include <QString>

struct __data{
    /* true enable */
    bool autosave : 1;
    bool temp_file : 1;

    uint t_autosave;
    uint t_temp_file;

    bool remove_file : 1; /* false do not remove the file after writernote close */

};

namespace Ui {
class setting_restore_ui;
}

class currenttitle_class;
class indice_class;

class setting_restore_ui : public QDialog
{
    Q_OBJECT

public:
    explicit setting_restore_ui(QWidget *parent = nullptr,
                                currenttitle_class **curr = nullptr,
                                indice_class *ind = nullptr,
                                QString *pp = nullptr);
    ~setting_restore_ui();

    struct __data getData () const
    {
        return m_data;
    }

    void setData(struct __data *data){
        memcpy(&m_data, data, sizeof(struct __data));
        updateWindow();
    }

    /* call this function when change copybook or close writernote */
    void changeCopybookFile(){
        if(m_data.remove_file
                && m_data.temp_file
                && (*m_path != ""
                || tmp_path != "")){
            deleteFile();
        }
    }

private:

    void deleteFile();

    QString tmp_path;
    QString *m_path;

    void startTimerSetting();

    QTimer *m_first;
    QTimer *m_sec;

    currenttitle_class **m_curr;
    indice_class *m_ind;

    void updateWindow();

    struct __data m_data;

    void loadData();
    void saveData();

    Ui::setting_restore_ui *ui;

signals:
    /* emit when user change somethings */
    void dataChange();

private slots:
    /* autosave */
    void firstTimer();

    /* tmp file */
    void secondTimer();

    void on_pushButton_ok_clicked();
    void on_pushButton_close_clicked();
    void on_spinBox_temp_valueChanged(int arg1);
    void on_spinBox_autosave_valueChanged(int arg1);
    void on_checkBox_autosave_stateChanged(int arg1);
    void on_checkBox_temp_stateChanged(int arg1);
    void on_checkBox_remove_stateChanged(int arg1);
};

#endif // SETTING_RESTORE_UI_H
