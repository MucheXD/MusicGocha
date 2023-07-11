#pragma once
#include <QWidget>
#include "ui_RootWindow.h"

class RootWindowU : public QWidget
{
	Q_OBJECT

public:
	RootWindowU();

private:
	Ui::RootWindowW ui_rootWindow;
	void changePage();
};