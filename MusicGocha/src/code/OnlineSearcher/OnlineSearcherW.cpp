#include "OnlineSearcherW.h"

OnlineSearcherW::OnlineSearcherW()
{
	ui_os.setupUi(this);

	connect(ui_os.edit_searchInput, &QLineEdit::returnPressed, ui_os.pb_startSearching, &QPushButton::animateClick);
	connect(ui_os.pb_startSearching, &QPushButton::clicked, this, &OnlineSearcherW::startSearching);
}

void OnlineSearcherW::startSearching()
{
	//TODO 业务逻辑<OnlineSearch>从此开始
	DialogBox::popMessageBox(this, "这个功能未完成", "程序无法继续执行，关键进程已退出", DialogBox::icon_critical
		, DialogBox::btn_abort, true);
}