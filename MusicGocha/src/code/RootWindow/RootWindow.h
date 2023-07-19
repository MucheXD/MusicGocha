#pragma once
#include <QWidget>
#include "../basics.h"
#include "ui_RootWindow.h"

#include "../OnlineSearcher/OnlineSearcherC.h"

enum FuncpagesENUM
{
	NULLPAGE = 0,
	OnlineSearcher = 1,
	LocalCollector = 2,
	ItemCart = 3,
	WorkQueue = 4,
	Options = 5,
};

struct FuncpagesPointer
{
	FuncpagesENUM currentFuncPage = FuncpagesENUM::NULLPAGE;
	OnlineSearcherC* onlineSearcher = NULL;
};

class RootWindow : public QWidget
{
	Q_OBJECT

public:
	RootWindow();

private:
	Ui::RootWindowU ui_rw;
	FuncpagesPointer funcPages_pointer;
	bool reloadStyleSheet();
	void changePage();
};