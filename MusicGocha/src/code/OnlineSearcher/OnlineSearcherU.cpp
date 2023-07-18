#include "OnlineSearcherU.h"

OnlineSearcherU::OnlineSearcherU()
{
	ui_os.setupUi(this);

	connect(ui_os.edit_searchInput, &QLineEdit::returnPressed, ui_os.pb_startSearching, &QPushButton::animateClick);
	connect(ui_os.pb_startSearching, &QPushButton::clicked, this, &OnlineSearcherU::startSearching);
}

void OnlineSearcherU::startSearching()
{
	qDebug() << QApplication::applicationDirPath();
}