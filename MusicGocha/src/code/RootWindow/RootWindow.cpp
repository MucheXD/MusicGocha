#include "RootWindow.h"

RootWindow::RootWindow()
{
	//创建及初始化必要实例对象
	ui_rw.setupUi(this);

	//连接tabbar菜单按钮的信号
	connect(ui_rw.pb_tabbar_onlineSearcher,&QPushButton::clicked,this,&RootWindow::changePage);
	connect(ui_rw.pb_tabbar_localCollector, &QPushButton::clicked, this, &RootWindow::changePage);
	connect(ui_rw.pb_tabbar_itemCart, &QPushButton::clicked, this, &RootWindow::changePage);
	connect(ui_rw.pb_tabbar_workQueue, &QPushButton::clicked, this, &RootWindow::changePage);
	connect(ui_rw.pb_tabbar_options, &QPushButton::clicked, this, &RootWindow::changePage);

	//设置首页默认为OnlineSearcher
	ui_rw.pb_tabbar_onlineSearcher->setChecked(true);
	changePage();

	reloadStyleSheet();
	this->show();
}

bool RootWindow::reloadStyleSheet()
{
	QFile file;
	file.setFileName("configs/style.qss");//TODO 此处应该从配置获取，但目前配置键值管理器还未实现
	if (!file.open(QIODevice::ReadOnly) || file.size()>= 0x100000)//大小大于1MB或打开失败均视作失败
		return false;
	this->setStyleSheet(file.readAll());
	file.close();
	return true;
}

void RootWindow::changePage()
{
	FuncpagesENUM call_page = OnlineSearcher;//默认为OnlineSearcher，除非程序BUG否则用不到
	//获取当前选择页面
	if (ui_rw.pb_tabbar_onlineSearcher->isChecked())
		call_page = FuncpagesENUM::OnlineSearcher;
	else if (ui_rw.pb_tabbar_localCollector->isChecked())
		call_page = FuncpagesENUM::LocalCollector;
	else if (ui_rw.pb_tabbar_itemCart->isChecked())
		call_page = FuncpagesENUM::ItemCart;
	else if (ui_rw.pb_tabbar_workQueue->isChecked())
		call_page = FuncpagesENUM::WorkQueue;
	else if (ui_rw.pb_tabbar_options->isChecked())
		call_page = FuncpagesENUM::Options;
	//如果页面不变则直接返回
	if (!funcPages_pointer.currentFuncPage == FuncpagesENUM::NULLPAGE)
		if (call_page == funcPages_pointer.currentFuncPage)
			return;
	//向不是将调用的页面发送删除请求，是否执行内存释放由对应类自行判断
	if (call_page != FuncpagesENUM::OnlineSearcher && funcPages_pointer.onlineSearcher != NULL)
		if (funcPages_pointer.onlineSearcher->tryDelete())
			funcPages_pointer.onlineSearcher = NULL;
	//TODO 由于页面未完成，此处缺失
	
	//调用页面
	if (call_page == FuncpagesENUM::OnlineSearcher)
	{
		if (funcPages_pointer.onlineSearcher == NULL)
			funcPages_pointer.onlineSearcher = new OnlineSearcherC(ui_rw.w_funcWorkspace);
		funcPages_pointer.onlineSearcher->showWidget();
	}
	//TODO 由于页面未完成，此处缺失
}