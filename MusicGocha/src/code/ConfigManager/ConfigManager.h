#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include "../basics.h"

class ConfigManager
{
public:
	enum ErrorList
	{
		no_error = 0,
		file_not_exist,
		cannot_open_file,
		illegal_config,
		manifest_mismatching,
		version_incompatible,
		illegal_data
	};

	ConfigManager();
	void setConfigFileName(QString fileName);
	ErrorList loadConfig();
	bool saveConfig(bool needBuildNew = false);
	QVariant getConfigValue(QString key);
	void setConfigValue(QString key, QVariant value);
	QVariant getDefaultValue(QString key);
	void fillDefaultValue();

private:

	struct ConfigElement
	{
		QString key = "";
		QVariant value = NULL;
	};

	QJsonDocument buildJsonConfig();
	std::vector<ConfigElement> parseJsonConfig(QJsonObject obj_root);

	QString configFileName;
	ErrorList error;
	std::vector<ConfigElement> configs;
	std::vector<ConfigElement> defaultConfigValues;
};