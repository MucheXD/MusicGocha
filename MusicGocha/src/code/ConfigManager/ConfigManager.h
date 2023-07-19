#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include "../basics.h"

class ConfigManager
{
public:
	ConfigManager();
	void setConfigFileName(QString fileName);
	bool loadConfig();
	bool saveConfig(bool needBuildNew = false);
	QVariant getConfigValue(QString key);

private:

	struct ConfigDisplay
	{
		QString styleSheetFileName = "";
	};
	struct ConfigList
	{
		ConfigDisplay display;
	};


	QString configFileName;
	ConfigList configs;

	QJsonDocument buildJsonConfig();
	bool writeConfigToFile(QJsonDocument data_jsonDoc);
};