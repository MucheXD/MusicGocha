#pragma once
#include <QTimer>

#include "ui_OnlineSearcherW.h"
#include "OnlineSearcherC.h"

class OnlineSearcherU : public QWidget
{
public:
	OnlineSearcherU(QWidget* parent);
	bool tryDelete();
private:
	Ui::OnlineSearcherW ui_os;
	//void tryReleaseMemory();
};