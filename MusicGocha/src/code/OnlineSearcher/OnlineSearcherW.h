#pragma once

#include "ui_OnlineSearcherU.h"
#include "ui_w_SearchResultU.h"
#include "../widgets/DialogBox.h"
#include "MusicInfoDefinition.h"

class OnlineSearcherW : public QWidget
{

	Q_OBJECT

public:
	OnlineSearcherW(std::vector<MusicGroup> const& groupsRef, std::vector<MusicInfo> const& infosRef);
	void updateUi();
private:
	Ui::OnlineSearcherU ui_os;
	std::vector<MusicGroup> const& musicGroups;
	std::vector<MusicInfo> const& musicInfos;
	void startSearching();
	void updateResultZone();
	//void tryReleaseMemory();
signals:
	void _startSearching(QString keyword, QString methodId);
};

class SearchResultWidgetU : public QWidget
{

	Q_OBJECT

public:

	SearchResultWidgetU();
	void setBasicInfo(QString title, QString information);

private:
	Ui::w_SearchResultU ui_w_sr;
};