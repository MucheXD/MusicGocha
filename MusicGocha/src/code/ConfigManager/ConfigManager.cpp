#include "ConfigManager.h"

ConfigManager::ConfigManager()
{
	setConfigFileName("configs/config.mtd");//设置默认配置文件位置
}

void ConfigManager::setConfigFileName(QString fileName)
{
	configFileName = fileName;
}

bool ConfigManager::loadConfig()
{
	QFile configFile;
	configFile.setFileName(configFileName);
	if (!configFile.open(QIODevice::ReadOnly) || configFile.size() >= 0x100000)
		return false;
	QString text_oriConfig = configFile.readAll();
	configFile.close();
	QString text_configHeader;
	QRegularExpression regExp;
	if (!bText_between(&text_configHeader, &text_oriConfig, "<manifest>", "</manifest>"))
		return false;
	regExp.setPattern("(?<=\\[for=)\\S*(?=\\])");
	if (regExp.match(text_configHeader).captured() != PROGRAMTEXTID)//配置与程序不符
		return false;
	regExp.setPattern("(?<=\\[createrVersion=)\\S*(?=\\])");
	if (regExp.match(text_oriConfig).captured().toUShort() != PROGRAMBUILDVER)//配置与版本不符//TODO此处限制了唯一版本，应有版本兼容性
		return false;
	QString text_configData;
	if(!bText_between(&text_configData,&text_oriConfig, "<data>", "</data>"))
	regExp.setPattern("(?<=\\[eptBlocks=)\\S*(?=\\])");
	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc.fromJson(text_configData.toStdString().c_str(), &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		return false;
	//开始解析键值
	QJsonObject obj_main;//配置根(/)

	QJsonObject obj_onlineSearchEngines = obj_main.value("OnlineSearchEngines").toObject();

	QJsonObject obj_localCollectorEngines = obj_main.value("LocalCollectorEngines").toObject();

	QJsonObject obj_workflowEngines = obj_main.value("WorkflowEngines").toObject();

	QJsonObject obj_working = obj_main.value("Working").toObject();

	QJsonObject obj_display = obj_main.value("Display").toObject();
	configs.display.styleSheetFileName = obj_display.value("StyleSheet.Filename").toString();

	return true;
}

bool ConfigManager::saveConfig(bool needBuildNew)
{
	if (needBuildNew == false)
	{
		return writeConfigToFile(buildJsonConfig());
	}
	else
	{
		//重置文件
		QFile configFile;
		configFile.setFileName(configFileName);
		configFile.remove();
		if (!configFile.open(QIODevice::WriteOnly))//将自动创建文件
			return false;//创建文件失败
		configFile.close();
		//写入空数据
		QJsonObject obj_empty;
		QJsonDocument obj_emptyJsonDoc;
		obj_emptyJsonDoc.setObject(obj_empty);
		return writeConfigToFile(obj_emptyJsonDoc);
	}
	return false;//模范代码: 为永远走不到的路径规定返回值o(*￣︶￣*)o
}

QVariant ConfigManager::getConfigValue(QString key)
{
	if (key == "Display.StyleSheet.Filename") return configs.display.styleSheetFileName;
	return NULL;
}

QJsonDocument ConfigManager::buildJsonConfig()
{
	QJsonObject obj_main;//配置根(/)

	QJsonObject obj_onlineSearchEngines;//组:在线搜索引擎配置(/OnlineSearchEngines/)
	obj_main.insert("OnlineSearchEngines", obj_onlineSearchEngines);

	QJsonObject obj_localCollectorEngines;//组:本地收集器引擎配置(/LocalCollectorEngines/)
	obj_main.insert("LocalCollectorEngines", obj_localCollectorEngines);

	QJsonObject obj_workflowEngines;//组:本地收集器引擎配置(/WorkflowEngines/)
	obj_main.insert("WorkflowEngines", obj_workflowEngines);

	QJsonObject obj_working;//组:工作行为(/Working/)
	obj_main.insert("Working", obj_working);

	QJsonObject obj_display;//组:显示行为(/Display/)
	obj_display.insert("StyleSheet.Filename", configs.display.styleSheetFileName);
	obj_main.insert("Display", obj_display);

	QJsonDocument data_jsonDoc;
	data_jsonDoc.setObject(obj_main);
	data_jsonDoc.toJson(QJsonDocument::Compact);

	return data_jsonDoc;
}

bool ConfigManager::writeConfigToFile(QJsonDocument data_jsonDoc)
{
	//构造配置文件内容
	QString data_config = "";
	data_config += QString("<manifest>[for=%1][createrVersion=%2]</manifest><data>%3</data>")
		.arg(PROGRAMTEXTID)
		.arg(PROGRAMBUILDVER)
		.arg(data_jsonDoc.toJson(QJsonDocument::Compact));

	//开始写文件
	QFile configFile;
	configFile.setFileName(configFileName);
	if (!configFile.open(QIODevice::ReadWrite) || configFile.size() >= 0x100000)
		return false;
	if (configFile.write(data_config.toUtf8()) != data_config.toUtf8().size())
		return false;
	return true;
}