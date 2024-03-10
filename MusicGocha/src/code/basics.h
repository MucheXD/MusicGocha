#pragma once
#include <QString>
#include <QFile>
#include <QDir>

const QString PROGRAMTEXTID = "MUG_MusicGocha";
const uint16_t PROGRAMBUILDVER = 0xA000;

bool bText_between(QString& ret, const QString& text, QString left, QString right, int from = 0);
bool bText_before(QString& ret, const QString& text, QString stop, int from = 0);
bool bText_after(QString& ret, const QString& text, QString start, int from = 0);

class IncrementInt32 { //byAI
private:
    mutable int32_t value;  // 使用int32_t替代int
public:
    IncrementInt32(int32_t init_val = 0) : value(init_val) {}
    int32_t operator()() const {
        return ++value;
    }
};
