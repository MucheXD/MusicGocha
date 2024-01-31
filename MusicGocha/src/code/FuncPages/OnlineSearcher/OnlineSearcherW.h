#pragma once

#include <QEvent>
#include <QMouseEvent>

#include "SearchResultDetailedInfoW.h"
#include "ui_OnlineSearcherU.h"
#include "ui_w_SearchResultU.h"
#include "../../widgets/DialogBox.h"
#include "../../definitions/MusicInfoDefinition.h"
#include "../../widgets/ShadowMask.h"

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
	SearchResultDetailedInfoW* detailedInfoW;
	ShadowMask* shadowMask;
	void startSearching();
	void updateResultZone();
	void resultWidgetBodyClicked(int32_t index);
	void detailedInfoWidgetClosed();
	void resizeEvent(QResizeEvent* resizeEvent);
	//void tryReleaseMemory();
signals:
	void _startSearching(QString keyword, QString methodId);
	void _needInfoCompleted(std::vector<MusicInfo*> requestedItem);
};

class SearchResultWidgetU : public QWidget
{

	Q_OBJECT

public:

	SearchResultWidgetU(int32_t index);
	void setBasicInfo(QString title, QString information);

private:
	Ui::w_SearchResultU ui_w_sr;
	int32_t widgetIndex;

protected:
	void mousePressEvent(QMouseEvent* mouseEvent);

signals:
	void _clicked(int32_t index);
};

