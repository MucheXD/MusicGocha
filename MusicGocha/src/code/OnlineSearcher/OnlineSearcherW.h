#pragma once

#include "ui_OnlineSearcherU.h"
#include "../widgets/DialogBox.h"

class OnlineSearcherW : public QWidget
{
public:
	OnlineSearcherW();
private:
	Ui::OnlineSearcherU ui_os;
	void startSearching();
	//void tryReleaseMemory();
};