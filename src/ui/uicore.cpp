#include "uicore.h"
#include "core/core.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/common_script.h"

static void adjustButtonDark(QAction *action, const QString &_path)
{
    constexpr auto suff = "_black_theme";
    constexpr auto find = ".png";
    constexpr auto not_used name = "adjustButtonDark";
    constexpr auto not_used debug = true;

    QString path = _path;
    path = path.insert(_path.indexOf(find), suff);

    action->setIcon(QIcon(path));
    WDebug(debug, name << path);
}

void UiCore::makePop(QWidget *widget)
{
    widget->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}

void UiCore::makeZeroBorder(QWidget *widget)
{
    W_ASSERT(widget);
    const auto current = widget->styleSheet();
    const auto style = qstr("QWidget { border : 0px solid black;}");
    widget->setStyleSheet(current + style);
}

void UiCore::adjustDarkMode()
{
    auto *main = core::get_main_window()->ui;
    adjustButtonDark(main->actionpen, ":/image/images/pen-icon.png");
    adjustButtonDark(main->actionrubber, ":/image/images/rubber.png");
    adjustButtonDark(main->actioninsertText, ":/image/images/insertText.png");
    adjustButtonDark(main->actionselezionetext, ":/image/images/cisors-cut.png");
    adjustButtonDark(main->actioninsertText, ":/image/images/insertText.png");
    adjustButtonDark(main->actioninsertImage, ":/image/images/image.png");
    adjustButtonDark(main->actionlastStyle, ":/image/images/new-sheet.png");
    adjustButtonDark(main->actionPen_or_Mouse, ":/image/images/pen_option.png");
    adjustButtonDark(main->actionHide_Show_Preview, ":/image/images/img_show_hide_preview.png");
    adjustButtonDark(main->actionFull_Screen, ":/image/images/full-screen.png");
    adjustButtonDark(main->actionChange_visual, ":/image/images/tablet.png");
    adjustButtonDark(main->actionhighlighter, ":/image/images/highlighter.png");
    adjustButtonDark(main->actionListen_current_audio, ":/image/images/manoIcon.png");
    adjustButtonDark(main->actionImport_Video, ":/image/images/importVideo.png");
    adjustButtonDark(main->actionDelete_audio, ":/image/images/deleteAudio.png");
    adjustButtonDark(main->stop_rec, ":/image/images/stopRecord_.png");
    adjustButtonDark(main->pause_rec, ":/image/images/pause-recording.png");
    adjustButtonDark(main->actionnewPage, ":/image/images/newPage.png");
    adjustButtonDark(main->actionNew_File, ":/image/images/newFileBig.png");
}
