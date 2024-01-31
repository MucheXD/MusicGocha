#pragma once
#include <QDatetime>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "OnlineSearcherW.h"
#include "../../widgets/DialogBox.h"
#include "OnlineSearchEngine.h"
#include "../FuncPageABLE.h"
#include "../../definitions/WorkRequestDefinition.h"



class OnlineSearcherC : public FuncPageABLE
{
	Q_OBJECT
public:
	OnlineSearcherC();

	void showWidget();
	bool tryDelete();
	void setWidgetParent(QWidget* parent);
	QWidget* getWidgetPointer();

	void assembleSearchEngines();
	void startSearching(QString keyword, QString methodId);
	
private:
	OnlineSearcherW* widget_os;
	//std::vector<OnlineSearcherScript> searcherScripts;
	std::vector<OnlineSearchEngine*> engines;
	std::vector<MusicInfo> musicInfoDatabase;
	std::vector<MusicGroup> musicGroups;
	void engineFinished(EngineTaskTarget targetType);
	//根据MUI数据库重新构建分组
	void GroupMusicInfos();
	//向分组内添加新的MUI。注意：为了避免野指针，删除MUI时不得使用此重载。
	void GroupMusicInfos(std::vector<MusicInfo> const& newMusicInfo);
	QNetworkReply* pushRequest_getNetworkReplyGET(QNetworkRequest& request);
signals:
	QVariant _fetchConfigValue(QString key);
	QNetworkReply* _getNetworkReplyGET(QNetworkRequest& request);
	void _addWorkToWorkCenter(std::vector<WorkRequest> requests);
};


