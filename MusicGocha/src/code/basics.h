#pragma once
#include <QString>
#include <QFile>

const QString PROGRAMTEXTID = "MUG_MusicGocha";
const int16_t PROGRAMBUILDVER = 0xA000;

bool bText_between(QString* ret, const QString* text, QString left, QString right, int from = 0);