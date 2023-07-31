#pragma once
#include "basics.h"

#include "ConfigManager/ConfigManager.h"
#include "RootWindow/RootWindow.h"

class ControlHubCore : public QObject
{
public:
	ControlHubCore();
	~ControlHubCore();
	bool launchApp();
	QVariant getConfigValue(QString key);
	void setConfigValue(QString key, QVariant value);
private:

	bool loadStyleSheet();

	QString globalStyleSheet;
	RootWindow* rootWindow;
	ConfigManager configManager;
};