#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QUrl>
#include <QDebug>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QFileDialog>
#include <QMediaRecorder>
#include <QStandardPaths>
#include <iostream>
#include <QComboBox>
#include <QMediaPlayer>
#include "touch/tabletcanvas.h"
#include "cloud/cloud_controll.h"
#include <QDropEvent>
#include <QBuffer>
#include "windows/updatecheck.h"
#include "currenttitle/document.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "touch/highlighter/highlighter.h"
#include "cloud/struct_user.h"

class frompdf;
class TabletApplication;
class option_copybook;
class ControllUiButton;
class xmlstruct;
class ui_scroll;
class setting_restore_ui;
#ifdef ANDROID_WRITERNOTE
class ShareUtils;
#endif

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)
#define VERSION_STRING STRINGIZE(VERSION_SNAPCRAFT)
#define REDO_UNDO_DISABLE "Disable redo undo"
#define REDO_UNDO_ENABLE "Enable redo undo"
#define NAME_UNSAVE (QString)"Unsaved copybook"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class AudioRecorder; }
class QAudioRecorder;
class QAudioProbe;
class QAudioBuffer;

QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /* -1 load from QSettings, -2 for show a slider */
    void setSizeButton(int size = -1);
    void resFileTmpInTmpFolder();
    void contrUi();
    void updateTouch();

    QString getNameCopybook();
    void updateTitle();

    enum n_need_save: uchar{
        unable_load, /*we had trouble loading the file*/
        not_, /* we don't have to save the file */
        need_save, /* we need to save */
        only_writernote, /* writernote change something insede */
        no_path
    };

    enum n_need_save needToSave(xmlstruct &xml,
                                Document & tmp_read) const;

    MainWindow(QWidget *parent = nullptr,
               TabletCanvas *canvas = nullptr,
               struct struct_user *user = nullptr,
               cloud_controll *cloud = nullptr,
               const char *open = nullptr);
    ~MainWindow();

    /* false -> use pen
    *  true -> use touch for drawing
    */
    bool touch_or_pen = false;
    void update_touch_or_pen();
    void loadPenOrMouse();

    /* data to manage all */
    int m_currentTime = 0;
    QString m_path;

    Document *m_currenttitle;
    Ui::MainWindow *ui;

    QAudioRecorder *m_audioRecorder = new QAudioRecorder(this);
    //QAudioProbe *m_probe = nullptr;
    bool m_outputLocationSet = false;

    /* gestione della riproduzione dell'audio */
    QMediaPlayer *player = nullptr;

    TabletCanvas *m_canvas;

    updatecheck *checkupdate;

    /* false -> not enable */
    bool enableredoundo = true;


    rubber_ui *m_rubber;
    pen_ui *m_pen;
    text_ui *m_text;
    highlighter *m_highlighter;
    text_widgets *m_text_w;
    option_copybook *m_option_copybook;
    struct struct_user *m_user;
    cloud_controll *m_cloud;
    QBuffer *m_buffer = nullptr;
    fast_sheet_ui *m_sheet = nullptr;
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    ShareUtils *m_share_file;
#endif

    void updatePageCount(int);
    setting_restore_ui *m_setting;

    void openFile(const char *pos);

    friend class ControllUiButton;
    ControllUiButton *m_controllUi;

public slots:
    void on_actioncompress_video_triggered();

    /* call by savefrom */
    void on_actionSave_File_triggered();

private slots:
    /* registrazione dell'audio */
    bool setOutputLocation();
    void togglePause();
    void on_actionNew_File_triggered();
    void closeEvent (QCloseEvent *event) override;
    void on_actionOpen_triggered();

    /* funzione che gestisce l'aggiornamento della status bar */
    void updateProgress(qint64 duration);

    /* funzione che gestisce la visualizzazione di un errore per la registrazione del video */
    void displayErrorMessage();

    /* riascolto audio */
    /* funzione che gestisce l'inserimento del testo formattato correttamente */
    void riascoltoaudioprogressivo(qint64 position);
    void on_actionListen_current_audio_triggered();

    /* funzione che gestisce la fine del riascolto dell'audio */
    void cambiostatoplayer(QMediaPlayer::State);
    void on_actionQuality_setting_triggered();
    void on_actionDelete_audio_triggered();
    void on_volumeSlider_actionTriggered(int action);
    void on_actionPrint_triggered();
    void on_audioSlider_valueChanged(int value);
    void on_volumeSlider_valueChanged(int value);
    void on_actionRedo_triggered();
    void on_actionpen_triggered();
    void on_actionrubber_triggered();
    void on_actionblack_triggered();
    void on_actionwhite_triggered();
    void on_actionyellow_triggered();
    void on_actionred_triggered();
    void on_actionselezionetext_triggered();
    void on_actionSave_As_triggered();
    void on_actionSource_code_triggered();
    void on_actionCheck_update_triggered();
    void on_actioninsertImage_triggered();
    void on_actionnew_sheet_triggered();
    void on_actionEnable_redo_undo_triggered();
    void on_actionchoose_color_triggered();
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
    void on_actionhighlighter_triggered();
    void on_actionCopybook_triggered();
    void on_actionbrown_color_triggered();
    void on_actionpurple_triggered();
    void on_start_rec_triggered();
    void on_pause_rec_triggered();
    void on_stop_play_clicked();
    void on_start_play_clicked();
    void on_stop_rec_triggered();
    void on_actionRemove_current_PDF_triggered();
    void on_actionSize_button_triggered();
    void on_actionRecent_file_triggered();
    void on_buttonRecentFile_clicked();
    void on_actionChange_visual_triggered();
    void on_buttonChangeVisual_clicked();
    void on_buttonPen_clicked();
    void on_buttonRubber_clicked();
    void on_buttonselezionetext_clicked();
    void on_buttonInsertText_clicked();
    void on_buttonHighlighter_clicked();
    void on_buttonBlack_clicked();
    void on_buttonWhite_clicked();
    void on_buttonYellow_clicked();
    void on_buttonBrown_clicked();
    void on_buttonPurple_clicked();
    void on_buttonChooseColor_clicked();
    void on_buttonInsertImage_clicked();
    void on_buttonSheet_clicked();
    void on_buttonNewPage_clicked();
    void on_buttonRestore_clicked();
    void on_buttonPenOrMouse_clicked();
    void on_buttonStartRecording_clicked();
    void on_buttonStopRecording_clicked();
    void on_buttonPauseRecording_clicked();
    void on_buttonListen_current_audio_clicked();
    void on_buttonUndu_clicked();
    void on_buttonRedo_clicked();
    void on_sliderZoom_sliderMoved(int position);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

signals:
    void UndoT();
    void RedoT();

};
#endif // MAINWINDOW_H
