#pragma once

#include "ui_OnlineSearcherU.h"
#include "../widgets/DialogBox.h"

class OnlineSearcherW : public QWidget
{

Q_OBJECT

public:
	OnlineSearcherW();
private:
	Ui::OnlineSearcherU ui_os;
	void startSearching();
	//void tryReleaseMemory();
signals:
	void _startSearching(QString keyword, int16_t mode_id);
};