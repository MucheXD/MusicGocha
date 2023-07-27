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
	if (DialogBox::popMessageBox(this
		, "这个功能未完成"
		, "程序无法继续执行，关键进程已退出\n如果你单击[重试]，可能会发生一些意外的事情"
		, DialogBox::icon_critical
		, DialogBox::btn_abort | DialogBox::btn_retry, true)
		== DialogBox::btn_retry)
	{
		while (true)
		{
			DialogBox::popMessageBox(this, "o(*////▽////*)q\no(*////▽////*)q\no(*////▽////*)q", "我们无法继续了，为了使你不至于太过无聊，我们尝试设计了这个弹窗。"
				"如你所见，我的换行键坏了，所以我不可能换行，这也是没有办法的事情对吧 :P", DialogBox::icon_warning_critical
				, DialogBox::btn_none, true, true);
		}
	}
	
}