#include "OnlineSearcherW.h"

OnlineSearcherW::OnlineSearcherW(std::vector<MusicGroup> const& groupsRef, std::vector<MusicInfo> const& infosRef) :
	musicGroups(groupsRef), musicInfos(infosRef)
{
	ui_os.setupUi(this);
	detailedInfoW = NULL;
	shadowMask = new ShadowMask;
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
	int32_t nUpdatingIndex = 0;
	while (nUpdatingIndex < musicGroups.size())
	{
		SearchResultWidgetU* newSearchResultWidget = new SearchResultWidgetU(nUpdatingIndex);
		const MusicGroup& nReadingGroup = musicGroups.at(nUpdatingIndex);
		newSearchResultWidget->setBasicInfo(nReadingGroup.sharedTitle, nReadingGroup.sharedAblumName);
		connect(newSearchResultWidget, &SearchResultWidgetU::_clicked, this, &OnlineSearcherW::resultWidgetBodyClicked);

		QListWidgetItem* newListWidgetItem = new QListWidgetItem;
		newListWidgetItem->setSizeHint(QSize(0, 50));
		ui_os.lw_searchResultZone->addItem(newListWidgetItem);
		ui_os.lw_searchResultZone->setItemWidget(newListWidgetItem, newSearchResultWidget);
		nUpdatingIndex += 1;
	}
}

void OnlineSearcherW::resultWidgetBodyClicked(int32_t index)
{
	shadowMask->showMask(this);//显示窗口遮罩(背景灰色)
	emit _needInfoCompleted(musicGroups.at(index).includedMusics);//发送信息补充请求
	if (detailedInfoW != NULL)//如果上一个详细信息窗口还未被删除，先执行删除
	{
		detailedInfoW->deleteLater();
		detailedInfoW = NULL;
	}
	detailedInfoW = new SearchResultDetailedInfoW(this, musicGroups.at(index));
	connect(detailedInfoW, &SearchResultDetailedInfoW::_widgetClosed, this, &OnlineSearcherW::detailedInfoWidgetClosed);
	detailedInfoW->showWidget();
}

void OnlineSearcherW::detailedInfoWidgetClosed()
{
	detailedInfoW = NULL;
	shadowMask->hideMask();
}

void OnlineSearcherW::resizeEvent(QResizeEvent* resizeEvent)
{
	if (detailedInfoW != NULL)//如果存在详细信息窗口，要求它适应新的大小
		detailedInfoW->updateGeometryFollowingParent();
}

SearchResultWidgetU::SearchResultWidgetU(int32_t index)
{
	ui_w_sr.setupUi(this);
	widgetIndex = index;
}

void SearchResultWidgetU::setBasicInfo(QString title, QString information)
{
	ui_w_sr.lb_titleText->setText(title);
	ui_w_sr.lb_information->setText(information);
}

void SearchResultWidgetU::mousePressEvent(QMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton)
	{
		emit _clicked(widgetIndex);
	}
}

