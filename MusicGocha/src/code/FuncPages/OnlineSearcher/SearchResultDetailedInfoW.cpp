#include "SearchResultDetailedInfoW.h"

SearchResultDetailedInfoW::SearchResultDetailedInfoW(QWidget* parent,const MusicGroup& setData)
	:currentData(setData)
{
	parentWidget = parent;
	this->setParent(parent);
	ui_di.setupUi(this);
	
	connect(ui_di.btn_closeWindow, &QPushButton::clicked, this, &SearchResultDetailedInfoW::closeWidget);
}

void SearchResultDetailedInfoW::showWidget()
{
	updateGeometryFollowingParent();
	updateUi();
	this->show();
}

void SearchResultDetailedInfoW::updateGeometryFollowingParent()
{
	//设置窗口大小
	const int newWidget = parentWidget->width() * 0.85;
	const int newHeight = parentWidget->height() * 0.85;
	this->setGeometry((parentWidget->width() - newWidget) / 2,
		(parentWidget->height() - newHeight) / 2,
		newWidget, newHeight);
}

void SearchResultDetailedInfoW::updateUi()
{
	ui_di.lb_titleText->setText(currentData.sharedTitle);
	ui_di.lb_album->setText(currentData.sharedAblumName);
}

void SearchResultDetailedInfoW::closeWidget()
{
	this->hide();
	this->deleteLater();
	emit _widgetClosed();
}
