#include "RootWindow.h"

RootWindowU::RootWindowU()
{
	//创建及初始化必要实例对象
	ui_rw.setupUi(this);

	//连接tabbar菜单按钮的信号
	connect(ui_rw.pb_tabbar_onlineSearcher,&QPushButton::clicked,this,&RootWindowU::changePage);
	connect(ui_rw.pb_tabbar_localCollector, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rw.pb_tabbar_itemCart, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rw.pb_tabbar_workQueue, &QPushButton::clicked, this, &RootWindowU::changePage);
	connect(ui_rw.pb_tabbar_options, &QPushButton::clicked, this, &RootWindowU::changePage);

	//设置首页默认为OnlineSearcher
	ui_rw.pb_tabbar_onlineSearcher->setChecked(true);
	changePage();

	this->show();
}

void RootWindowU::changePage()
{
	FUNCPAGES_ENUM call_page = OnlineSearcher;//默认为OnlineSearcher，除非程序BUG否则用不到
	//获取当前选择页面
	if (ui_rw.pb_tabbar_onlineSearcher->isChecked())
		call_page = FUNCPAGES_ENUM::OnlineSearcher;
	else if (ui_rw.pb_tabbar_localCollector->isChecked())
		call_page = FUNCPAGES_ENUM::LocalCollector;
	else if (ui_rw.pb_tabbar_itemCart->isChecked())
		call_page = FUNCPAGES_ENUM::ItemCart;
	else if (ui_rw.pb_tabbar_workQueue->isChecked())
		call_page = FUNCPAGES_ENUM::WorkQueue;
	else if (ui_rw.pb_tabbar_options->isChecked())
		call_page = FUNCPAGES_ENUM::Options;
	//如果页面不变则直接返回
	if (!funcPages_pointer.currentFuncPage == FUNCPAGES_ENUM::NULLPAGE)
		if (call_page == funcPages_pointer.currentFuncPage)
			return;
	//向不是将调用的页面发送删除请求，是否执行内存释放由对应类自行判断
	//TODO 这里代码未来的重复率太高了，或许可以改成模板形式
	if (call_page != FUNCPAGES_ENUM::OnlineSearcher && funcPages_pointer.onlineSearcher != NULL)
		if (funcPages_pointer.onlineSearcher->tryDelete())
			funcPages_pointer.onlineSearcher = NULL;
	//TODO 由于页面未完成，此处缺失
	
	//调用页面
	if (call_page == FUNCPAGES_ENUM::OnlineSearcher)
	{
		if (funcPages_pointer.onlineSearcher == NULL)
			funcPages_pointer.onlineSearcher = new OnlineSearcherU(ui_rw.w_funcWorkspace);
		funcPages_pointer.onlineSearcher->show();
	}
	//TODO 由于页面未完成，此处缺失
}