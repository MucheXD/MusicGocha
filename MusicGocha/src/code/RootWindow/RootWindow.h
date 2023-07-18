#pragma once
#include <QWidget>
#include <QFile>
#include "ui_RootWindow.h"

#include "../OnlineSearcher/OnlineSearcherC.h"

enum FUNCPAGES_ENUM
{
	NULLPAGE = 0,
	OnlineSearcher = 1,
	LocalCollector = 2,
	ItemCart = 3,
	WorkQueue = 4,
	Options = 5,
};

struct FUNCPAGES_POINTER
{
	FUNCPAGES_ENUM currentFuncPage = FUNCPAGES_ENUM::NULLPAGE;
	OnlineSearcherC* onlineSearcher = NULL;
};

class RootWindowU : public QWidget
{
	Q_OBJECT

public:
	RootWindowU();

private:
	Ui::RootWindowW ui_rw;
	FUNCPAGES_POINTER funcPages_pointer;
	bool reloadStyleSheet();
	void changePage();
};