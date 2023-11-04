#pragma once
#include <QDatetime>
#include <QFile>
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
	void startSearching();
	
private:
	OnlineSearcherW* widget_os;
	std::vector<OnlineSearcherScript> searcherScripts;
signals:
	QVariant _fetchConfigValue(QString key);
};


