#include "ControlHub.h"

ControlHubCore::ControlHubCore()
{
	rootWindow = NULL;
	qDebug() << configManager.saveConfig();
}

bool ControlHubCore::launchApp()
{
	//TODO 配置读写任务1
	//启动RootWindow
	rootWindow = new RootWindow;
	return true;
}

QVariant ControlHubCore::getConfigValue(QString key)
{
	return configManager.getConfigValue(key);
}