#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QUrl>
#include "audiorecord/audiolevel.h"

/* gestione variabile self */
#include "self_class.h"

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QFileDialog>
#include <QMediaRecorder>
#include <QStandardPaths>
#include <QDebug>
#include <iostream>
#include <QComboBox>
#include <QMediaPlayer>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /* gestione variabile self */
    SelfClass *self;
    Ui::MainWindow *ui;

    QAudioRecorder *m_audioRecorder = new QAudioRecorder(this);
    QAudioProbe *m_probe = nullptr;
    QList<AudioLevel*> m_audioLevels;
    bool m_outputLocationSet = false;

    /* gestione della riproduzione dell'audio */
    QMediaPlayer *player;




public slots:
    void processBuffer(const QAudioBuffer&);

    /* la funzione serve anche per eliminare l'audio del copybook */
    void on_listWidgetSX_itemDoubleClicked(QListWidgetItem *item);

private slots:
    /* registrazione dell'audio */
    void setOutputLocation();
    void togglePause();

    void on_actionNew_File_triggered();
    void closeEvent (QCloseEvent *event);
    void on_actionSave_File_triggered();

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

    void on_actioncompress_video_triggered();

    void on_volumeSlider_actionTriggered(int action);

    void on_spinBox_fontsize_valueChanged(const QString &arg1);

    void on_fontComboBox_fonttipo_currentFontChanged(const QFont &f);

    void on_actionPrint_triggered();


private:
    /* gestione dell'audio */
    void clearAudioLevels();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

};
#endif // MAINWINDOW_H
