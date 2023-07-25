#pragma once
#include "OnlineSearcherW.h"
#include "../widgets/DialogBox.h"

class OnlineSearcherC
{
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
private:
	OnlineSearcherW* widget_os;
};