#ifndef CURRENT_TIME_H
#define CURRENT_TIME_H

#include <QDateTime>
#include <QTime>
#include <QDate>

QTime current_time();
QString current_time_string();

QDate current_day();
QString current_day_string();

#endif // CURRENT_TIME_H
