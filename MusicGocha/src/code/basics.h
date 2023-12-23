#pragma once
#include <QString>
#include <QFile>
#include <QDir>

const QString PROGRAMTEXTID = "MUG_MusicGocha";
const uint16_t PROGRAMBUILDVER = 0xA000;

bool bText_between(QString& ret, const QString& text, QString left, QString right, int from = 0);
bool bText_before(QString& ret, const QString& text, QString stop, int from = 0);
bool bText_after(QString& ret, const QString& text, QString start, int from = 0);