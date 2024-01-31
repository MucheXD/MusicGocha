#pragma once
#include "basics.h"
#include <QNetworkAccessManager>

#include "ConfigManager/ConfigManager.h"

#include "definitions/WorkRequestDefinition.h"
#include "RootWindow/RootWindow.h"
#include "FuncPages/OnlineSearcher/OnlineSearcherC.h"
#include "FuncPages/WorkCenter/WorkCenterC.h"


class ControlHubCore : public QObject
{
public:
	ControlHubCore();
	~ControlHubCore();
	bool launchApp();
private:

	QVariant getConfigValue(QString key);
	void setConfigValue(QString key, QVariant value);
	QNetworkReply* getNetworkReplyGET(QNetworkRequest& request);
	void addWorkToWorkCenter(std::vector<WorkRequest> requests);

	bool loadStyleSheet();
	void tryDeleteFuncPage(FuncpageENUM needDeletePage);
	FuncPageABLE* createFuncPage(FuncpageENUM needCreatePage, bool enforced = false);
	void createFuncPage(OnlineSearcherC*& creation);
	void createFuncPage(WorkCenterC*& creation);
	void changeFuncPage(FuncpageENUM newFuncPage);

	QString globalStyleSheet;

	RootWindow* rootWindow;
	struct {
		OnlineSearcherC* onlineSearcher = NULL;
		WorkCenterC* workCenter = NULL;
	}funcPages;

	ConfigManager configManager;
	QNetworkAccessManager networkAccessManager;

};