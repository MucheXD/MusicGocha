#include "RootWindow.h"

RootWindow::RootWindow()
{
	//创建及初始化必要实例对象
	ui_rw.setupUi(this);
	currentFuncPage = FuncpageENUM::NULLPAGE;

	//连接tabbar菜单按钮的信号
	connect(ui_rw.pb_tabbar_onlineSearcher,&QPushButton::clicked,this,&RootWindow::pageChangeNeeded);
	connect(ui_rw.pb_tabbar_localCollector, &QPushButton::clicked, this, &RootWindow::pageChangeNeeded);
	connect(ui_rw.pb_tabbar_itemCart, &QPushButton::clicked, this, &RootWindow::pageChangeNeeded);
	connect(ui_rw.pb_tabbar_workQueue, &QPushButton::clicked, this, &RootWindow::pageChangeNeeded);
	connect(ui_rw.pb_tabbar_options, &QPushButton::clicked, this, &RootWindow::pageChangeNeeded);

	this->show();
}

void RootWindow::updateCurrentFuncPage(FuncpageENUM current, FuncPageABLE* funcPage)
{
	currentFuncPage = current;
	funcPage->setWidgetParent(ui_rw.w_funcWorkspace);
	ui_rw.GL_workspaceSuit->addWidget(funcPage->getWidgetPointer());
	funcPage->showWidget();
}

void RootWindow::showWidget()
{
	if (currentFuncPage == FuncpageENUM::NULLPAGE)
	{
		//设置首页默认为OnlineSearcher
		ui_rw.pb_tabbar_onlineSearcher->setChecked(true);
	}
	pageChangeNeeded();
}

void RootWindow::pageChangeNeeded()
{
	FuncpageENUM newPage = FuncpageENUM::NULLPAGE;
	if(ui_rw.pb_tabbar_onlineSearcher->isChecked())
		newPage = FuncpageENUM::OnlineSearcher;
	else if (ui_rw.pb_tabbar_localCollector->isChecked())
		newPage = FuncpageENUM::LocalCollector;
	else if (ui_rw.pb_tabbar_workQueue->isChecked())
		newPage = FuncpageENUM::WorkCenter;
	else if (ui_rw.pb_tabbar_options->isChecked())
		newPage = FuncpageENUM::Options;
	//判断页面是否改变，如果页面不变则直接返回
	if ((!currentFuncPage == FuncpageENUM::NULLPAGE) && (newPage == currentFuncPage))
		return;
	else
		emit _changeFuncPage(newPage);
}



