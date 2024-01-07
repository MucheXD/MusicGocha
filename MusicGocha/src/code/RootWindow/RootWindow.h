#pragma once
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "../basics.h"
#include "ui_RootWindow.h"

#include "../OnlineSearcher/OnlineSearcherC.h"

enum FuncpagesENUM
{
	NULLPAGE = 0,
	OnlineSearcher = 1,
	LocalCollector = 2,
	ItemCart = 3,
	WorkQueue = 4,
	Options = 5,
};

struct FuncpagesPointer
{
	FuncpagesENUM currentFuncPage = FuncpagesENUM::NULLPAGE;
	OnlineSearcherC* onlineSearcher = NULL;
};

class RootWindow : public QWidget
{
	Q_OBJECT
public:
	RootWindow();
private:
	Ui::RootWindowU ui_rw;
	FuncpagesPointer funcPages_pointer;
	void changePage();
	QVariant pushRequest_fetchConfigValue(QString key);
	QNetworkReply* pushRequest_getNetworkReplyGET(QNetworkRequest& request);

signals:
	QVariant _fetchConfigValue(QString key);
	QNetworkReply* _getNetworkReplyGET(QNetworkRequest& request);
};