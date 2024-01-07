#pragma once
#include <QDatetime>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "OnlineSearcherW.h"
#include "../widgets/DialogBox.h"
#include "OnlineSearchEngine.h"



class OnlineSearcherC : public QObject
{
	Q_OBJECT
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
	QWidget* getWidgetPointer();

	void assembleSearchEngines();
	void startSearching(QString keyword, QString methodId);
	
private:
	OnlineSearcherW* widget_os;
	//std::vector<OnlineSearcherScript> searcherScripts;
	std::vector<OnlineSearchEngine*> engines;
	std::vector<MusicInfo> musicInfoDatabase;

	void engineFinished();
	void mergeToDatabase(std::vector<MusicInfo> data);
	QNetworkReply* pushRequest_getNetworkReplyGET(QNetworkRequest& request);
signals:
	QVariant _fetchConfigValue(QString key);
	QNetworkReply* _getNetworkReplyGET(QNetworkRequest& request);
};


