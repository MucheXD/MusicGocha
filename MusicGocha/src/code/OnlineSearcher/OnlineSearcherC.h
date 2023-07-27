#pragma once
#include "OnlineSearcherW.h"
#include "../widgets/DialogBox.h"

class OnlineSearcherC
{
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
	QWidget* getWidgetPointer();
private:
	OnlineSearcherW* widget_os;
};