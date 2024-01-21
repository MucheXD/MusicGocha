#include "OnlineSearcherW.h"

OnlineSearcherW::OnlineSearcherW(std::vector<MusicGroup> const& groupsRef, std::vector<MusicInfo> const& infosRef) :
	musicGroups(groupsRef), musicInfos(infosRef)
{
	ui_os.setupUi(this);
	connect(ui_os.edit_searchInput, &QLineEdit::returnPressed, ui_os.pb_startSearching, &QPushButton::animateClick);
	connect(ui_os.pb_startSearching, &QPushButton::clicked, this, &OnlineSearcherW::startSearching);
}

void OnlineSearcherW::startSearching()
{
	ui_os.pb_startSearching->setEnabled(false);
	emit _startSearching(ui_os.edit_searchInput->text(), "default");//TODO固定了搜索方式
	
	
}

void OnlineSearcherW::updateUi()
{
	//TODO 现在还没有DetailedInfoWidet
	updateResultZone();
}

void OnlineSearcherW::updateResultZone()
{
	for (MusicGroup nReading : musicGroups)
	{
		SearchResultWidgetU* newSearchResultWidget = new SearchResultWidgetU;
		newSearchResultWidget->setBasicInfo(nReading.sharedTitle, nReading.sharedAblumName);
		QListWidgetItem* newListWidgetItem = new QListWidgetItem;

		newListWidgetItem->setSizeHint(QSize(0,50));

		ui_os.lw_searchResultZone->addItem(newListWidgetItem);
		ui_os.lw_searchResultZone->setItemWidget(newListWidgetItem, newSearchResultWidget);
	}
	

}



SearchResultWidgetU::SearchResultWidgetU()
{
	ui_w_sr.setupUi(this);
}

void SearchResultWidgetU::setBasicInfo(QString title, QString information)
{
	ui_w_sr.lb_titleText->setText(title);
	ui_w_sr.lb_information->setText(information);
}