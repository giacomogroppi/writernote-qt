#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QUrl>

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QFileDialog>
#include <QMediaRecorder>
#include <QStandardPaths>
#include <iostream>
#include <QComboBox>
#include <QMediaPlayer>

/* for drag file into writernote */
#include <QDropEvent>

#include <QBuffer>

#include "windows/updatecheck.h"

#define REDO_UNDO_DISABLE "Disable redo undo"
#define REDO_UNDO_ENABLE "Enable redo undo"
#include "currenttitle/currenttitle_class.h"
#include "indice_class.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"

#include "cloud/struct_user.h"

class TabletApplication;

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)
#define VERSION_STRING STRINGIZE(VERSION_SNAPCRAFT)

#include "touch/tabletcanvas.h"
#include "cloud/cloud_controll.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class AudioRecorder; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;

QT_END_NAMESPACE

void addImage(currenttitle_class *m_currenttitle,
                     const char *__pos,
                     struct PointSettable * point);
class xmlstruct;
class ui_scroll;
class setting_restore_ui;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void updateTouch();

    enum n_need_save: uchar{
        unable_load, /*we had trouble loading the file*/
        not_, /* we don't have to save the file */
        need_save, /* we need to save */
        only_writernote /* writernote change something insede */
    };

    enum n_need_save needToSave(xmlstruct *xml,
                                currenttitle_class * tmp_read,
                                indice_class *tmp_ind) const;

    MainWindow(QWidget *parent = nullptr,
               TabletCanvas *canvas = nullptr,
               struct struct_user *user = nullptr,
               cloud_controll *cloud = nullptr);
    ~MainWindow();

    /* false -> use pen
    *  true -> use touch for drawing
    */
    bool touch_or_pen = false;
    void update_touch_or_pen();
    void loadPenOrMouse();

    QTimer *m_timer;

    QString m_currentTitle;

    /* data to manage all */
    int m_currentTime = 0;
    QString m_path;

    indice_class m_indice;
    currenttitle_class *m_currenttitle;
    Ui::MainWindow *ui;

    QAudioRecorder *m_audioRecorder = new QAudioRecorder(this);
    //QAudioProbe *m_probe = nullptr;
    bool m_outputLocationSet = false;

    /* gestione della riproduzione dell'audio */
    QMediaPlayer *player = nullptr;

    TabletCanvas *m_canvas;

    /* le due funzioni gestiscono il cambio di inserimento */
    void methodwrite();
    void methodpen();

    bool typetemp = false;

    updatecheck *checkupdate;

    void setFile(char *);

    void update_image();

    /* false -> not enable */
    bool enableredoundo = true;


    rubber_ui *m_rubber;
    pen_ui *m_pen;
    text_ui *m_text;

    text_widgets *m_text_w;

    struct struct_user *m_user;

    cloud_controll *m_cloud;

    /* manage audio player */
    QBuffer *m_buffer = nullptr;

    fast_sheet_ui *m_sheet = nullptr;

    void updatePageCount(int);
    setting_restore_ui *m_setting;

    void openFile(const char *pos);
    void update_list_copybook();

public slots:
    /* la funzione serve anche per eliminare l'audio del copybook */
    void on_listWidgetSX_itemDoubleClicked(QListWidgetItem *item);

    void on_actioncompress_video_triggered();

    /* call by savefrom */
    void on_actionSave_File_triggered();

private slots:
    /* registrazione dell'audio */
    bool setOutputLocation();
    void togglePause();

    void on_actionNew_File_triggered();
    void closeEvent (QCloseEvent *event) override;


    /* richiamata quando viene modificato qualcosa del testo */
    void on_textEdit_textChanged();

    void on_actionOpen_triggered();



    void on_textEdit_selectionChanged();

    void on_actionCreate_new_copybook_triggered();

    void on_startrecording_triggered();

    //void on_stoprecordingbotton_triggered();
    void on_pauserecordingbotton_triggered();

    /* funzione che gestisce l'aggiornamento della status bar */
    void updateProgress(qint64 duration);

    /* funzione che gestisce la visualizzazione di un errore per la registrazione del video */
    void displayErrorMessage();

    //void on_startriascolto_clicked();

    void on_stopriascolto_clicked();

    /* riascolto audio */
    /* funzione che gestisce l'inserimento del testo formattato correttamente */
    void riascoltoaudioprogressivo(qint64 position);
    void on_actionListen_current_audio_triggered();

    void on_startriascoltobotton_clicked();

    void on_boldbotton_clicked();

    void on_inserttablebotton_clicked();

    void on_insertimagebotton_clicked();

    void on_listbotton_clicked();

    void on_actionDelete_copybook_triggered();


    /* funzioni per gestire il click sull'item della lista */
    void deleteaudio_f();
    void deletecopybook_f();
    void audiototext_f();
    void renamefile_f();
    void compressvideo_f();



    /* funzione che gestisce la fine del riascolto dell'audio */
    void cambiostatoplayer(QMediaPlayer::State);

    void on_actionQuality_setting_triggered();

    void on_stoprecordingbotton_triggered();

    void on_actionDelete_audio_triggered();


    void on_volumeSlider_actionTriggered(int action);

    void on_spinBox_fontsize_valueChanged(const QString &arg1);

    void on_fontComboBox_fonttipo_currentFontChanged(const QFont &f);

    void on_actionPrint_triggered();


    void on_audioSlider_valueChanged(int value);

    void on_volumeSlider_valueChanged(int value);

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionSelect_all_triggered();

    void on_actionPaste_triggered();

    void on_actionCopy_triggered();

    void on_actionpen_triggered();

    void on_actionrubber_triggered();

    void on_actionblack_triggered();

    void on_actionwhite_triggered();

    void on_actionyellow_triggered();

    void on_actionred_triggered();

    void on_actionselezionetext_triggered();

    void on_actionSave_As_triggered();

    void on_actionSource_code_triggered();

    void on_insertimagebotton_triggered(QAction *arg1);

    void on_actionCheck_update_triggered();

    void on_actioninsertImage_triggered();

    void on_actionnew_sheet_triggered();

    void on_actionEnable_redo_undo_triggered();

    void on_actionchoose_color_triggered();

    void on_button_left_hide_clicked();

    void on_button_right_hide_clicked();

    void on_actioncloud_triggered();

    void on_actionVersion_triggered();

    void on_actionBug_report_triggered();

    void on_actiontopdf_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actioninsertText_triggered();

    void setInZipAudio();
    void setExtAudio();

    void on_actionDefault_save_location_triggered();

    void on_actionUndu_triggered();

    void on_actionExtract_audio_triggered();

    void on_actionlastStyle_triggered();

    void showRiascolto();

    void on_actionrestore_button_triggered();

    void on_actionnewPage_triggered();

    void on_actionDefault_method_write_triggered();

    void on_actionReduce_size_file_triggered();

    void on_actionChange_language_triggered();

    void on_actionScroll_triggered();

    void on_actionLast_open_file_triggered();

    void on_actionRestore_file_triggered();

    void on_actionOption_triggered();

    void on_actionUpdate_writernote_triggered();

    void on_actionPen_or_Mouse_triggered();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;


signals:
    void UndoT();
    void RedoT();


};
#endif // MAINWINDOW_H
