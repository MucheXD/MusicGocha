#pragma once
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "../basics.h"
#include "ui_RootWindow.h"

#include "../definitions/WorkRequestDefinition.h"
#include "../FuncPages/FuncPageABLE.h"

enum FuncpageENUM
{
	NULLPAGE = 0,
	OnlineSearcher = 1,
	LocalCollector = 2,
	WorkCenter = 3,
	Options = 4,
	ENDPAGE = 5 //为了方便遍历而设置，值必须为最大
};

class RootWindow : public QWidget
{
	Q_OBJECT
public:
	RootWindow();
	void updateCurrentFuncPage(FuncpageENUM current, FuncPageABLE* funcPage);
	void showWidget();
private:
	Ui::RootWindowU ui_rw;
	FuncpageENUM currentFuncPage;
	void pageChangeNeeded();
	QVariant pushRequest_fetchConfigValue(QString key);
	QNetworkReply* pushRequest_getNetworkReplyGET(QNetworkRequest& request);
	void pushRequest_addWorkToWorkCenter(std::vector<WorkRequest>);

signals:

	void _changeFuncPage(FuncpageENUM newFuncPageID);
};