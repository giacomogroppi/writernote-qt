#ifndef GETBUFFERLEVELS_H
#define GETBUFFERLEVELS_H
#include <QList>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDir>
#include <QFileDialog>
#include <QMediaRecorder>
#include <QStandardPaths>
#include <QDebug>
#include <iostream>
#include <QComboBox>

//static qreal getPeakValue(const QAudioFormat &);
QList<qreal> getBufferLevels(const QAudioBuffer &);

template<class T>
QList<qreal> getBufferLevels(const T *, int , int );

// This function returns the maximum possible sample value for a given audio format
//static qreal getPeakValue(const QAudioFormat& );

// returns the audio level for each channel
QList<qreal> getBufferLevels(const QAudioBuffer &);


template<class T>
QList<qreal> getBufferLevels(const T *, int , int );

#endif // GETBUFFERLEVELS_H
