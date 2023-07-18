#pragma once
#include "OnlineSearcherU.h"

class OnlineSearcherC
{
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
private:
	OnlineSearcherU* onlineSearcherU;
};