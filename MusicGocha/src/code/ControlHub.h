#pragma once
#include "basics.h"

#include "ConfigManager/ConfigManager.h"
#include "RootWindow/RootWindow.h"

class ControlHubCore
{
public:
	ControlHubCore();
	bool launchApp();
	QVariant getConfigValue(QString key);
private:
	RootWindow* rootWindow;
	ConfigManager configManager;
};