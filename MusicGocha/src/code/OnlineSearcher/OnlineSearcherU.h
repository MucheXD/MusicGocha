#pragma once

#include "ui_OnlineSearcherW.h"

class OnlineSearcherU : public QWidget
{
public:
	OnlineSearcherU();
private:
	Ui::OnlineSearcherW ui_os;
	void startSearching();
	//void tryReleaseMemory();
};