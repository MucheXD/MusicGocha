#pragma once
#include "OnlineSearcherW.h"

class OnlineSearcherC
{
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
private:
	OnlineSearcherW* widget_os;
};