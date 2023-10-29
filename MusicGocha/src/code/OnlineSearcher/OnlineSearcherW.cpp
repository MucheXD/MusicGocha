#include "OnlineSearcherW.h"

OnlineSearcherW::OnlineSearcherW()
{
	ui_os.setupUi(this);

	connect(ui_os.edit_searchInput, &QLineEdit::returnPressed, ui_os.pb_startSearching, &QPushButton::animateClick);
	connect(ui_os.pb_startSearching, &QPushButton::clicked, this, &OnlineSearcherW::startSearching);
}

void OnlineSearcherW::startSearching()
{
	ui_os.pb_startSearching->setEnabled(false);
	emit _startSearching(ui_os.edit_searchInput->text());
	
	
}