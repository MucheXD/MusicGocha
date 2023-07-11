#include "RootWindow.h"

RootWindowU::RootWindowU()
{
	ui_rootWindow.setupUi(this);

	//连接tabbar菜单按钮的信号
	connect(ui_rootWindow.pb_tabbar_onlineSearcher,&QPushButton::clicked,this,&RootWindowU::changePage);
	connect(ui_rootWindow.pb_tabbar_localCollector, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rootWindow.pb_tabbar_itemCart, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rootWindow.pb_tabbar_workQueue, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rootWindow.pb_tabbar_options, &QPushButton::clicked, this, &RootWindowU::changePage);

	//设置首页默认为OnlineSearcher
	ui_rootWindow.pb_tabbar_onlineSearcher->setChecked(true);
	changePage();

	this->show();
}

void RootWindowU::changePage()
{
	//定义页面ID
	enum
	{
		OnlineSearcher = 1,
		LocalCollector = 2,
		ItemCart = 3,
		WorkQueue = 4,
		Options = 0,
	}
	current_page = OnlineSearcher;
	//获取当前选择页面
	if (ui_rootWindow.pb_tabbar_onlineSearcher->isChecked())
		current_page = OnlineSearcher;
	else if (ui_rootWindow.pb_tabbar_localCollector->isChecked())
		current_page = LocalCollector;
	else if (ui_rootWindow.pb_tabbar_itemCart->isChecked())
		current_page = ItemCart;
	else if (ui_rootWindow.pb_tabbar_workQueue->isChecked())
		current_page = WorkQueue;
	else if (ui_rootWindow.pb_tabbar_options->isChecked())
		current_page = Options;
	
}