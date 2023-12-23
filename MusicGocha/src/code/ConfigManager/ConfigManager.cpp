#include "ConfigManager.h"

ConfigManager::ConfigManager()
{
	setConfigFileName("configs/config.mtd");//设置默认配置文件位置
	//默认值列表
	defaultConfigValues ={
	{"debug.test","DEBUG MESSAGE"}
	};
}

void ConfigManager::setConfigFileName(QString fileName)
{
	configFileName = fileName;
}

ConfigManager::ErrorList ConfigManager::loadConfig()
{
	QFile configFile;
	configFile.setFileName(configFileName);
	if (!configFile.exists())
		return ConfigManager::file_not_exist;
	if (!configFile.open(QIODevice::ReadOnly) || configFile.size() >= 0x100000)
		return ConfigManager::cannot_open_file;
	QString text_oriConfig = configFile.readAll();
	configFile.close();
	//校验配置文件基本信息
	QString text_configHeader;
	QRegularExpression regExp;
	if (!bText_between(text_configHeader, text_oriConfig, "<manifest>", "</manifest>"))
		return ConfigManager::illegal_config;
	regExp.setPattern("(?<=\\[for=)[^\\[]*(?=\\])");
	if (regExp.match(text_configHeader).captured() != PROGRAMTEXTID)//配置与程序不符
		return ConfigManager::manifest_mismatching;
	regExp.setPattern("(?<=\\[createrVersion=)[^\\[]*(?=\\])");
	if (regExp.match(text_oriConfig).captured().toUShort() != PROGRAMBUILDVER)//配置与版本不符//TODO此处限制了唯一版本，应有版本兼容性
		return ConfigManager::version_incompatible;
	//读取数据
	QString text_configData;
	if (!bText_between(text_configData, text_oriConfig, "<data>", "</data>"))
		return ConfigManager::illegal_config;

	QJsonDocument data_jsonDoc;
	QJsonParseError data_jsonError;
	data_jsonDoc = data_jsonDoc.fromJson(text_configData.toStdString().c_str(), &data_jsonError);
	if (data_jsonError.error != QJsonParseError::NoError)
		return ConfigManager::illegal_data;
	//解析键值
	configs = parseJsonConfig(data_jsonDoc.object());
	return ConfigManager::no_error;
}

bool ConfigManager::saveConfig(bool needBuildNew)
{
	QString data_config = QString("<manifest>[for=%1][createrVersion=%2]</manifest>")
		.arg(PROGRAMTEXTID)
		.arg(PROGRAMBUILDVER);
	if (needBuildNew == false)
		data_config+=QString("<data>%1</data>").arg(buildJsonConfig().toJson(QJsonDocument::Compact));
	else
	{
		//重置文件
		//创建目录(按需)
		if (configFileName.indexOf("/") != -1)
		{
			QDir fileDir;
			if (!fileDir.exists(configFileName.left(configFileName.lastIndexOf("/"))))
				if(!fileDir.mkdir(configFileName.left(configFileName.lastIndexOf("/"))))
					return false;//创建目录失败
		}
		QFile configFile;
		configFile.setFileName(configFileName);
		configFile.remove();
		if (!configFile.open(QIODevice::WriteOnly))//将自动创建文件
			return false;//创建文件失败
		configFile.close();
		//构造空数据
		data_config += QString("<data>{}</data>");
	}
	//开始写文件
	QFile configFile;
	configFile.setFileName(configFileName);
	if (!configFile.open(QIODevice::ReadWrite) || configFile.size() >= 0x100000)
		return false;
	if (configFile.write(data_config.toUtf8()) != data_config.toUtf8().size())
		return false;
	return true;
}

QVariant ConfigManager::getConfigValue(QString key)
{
	for (ConfigManager::ConfigElement n : configs)
		if (n.key == key)
			return n.value;
	return NULL;
}

void ConfigManager::setConfigValue(QString key, QVariant value)
{
	for (ConfigManager::ConfigElement &n : configs)//遍历检查，如果已有key则执行修改修改
		if (n.key == key)
		{
			n.value = value;
			return;
		}
	//找不到对应key，执行创建
	ConfigManager::ConfigElement element;
	element.key = key;
	element.value = value;
	configs.push_back(element);
}

QJsonDocument ConfigManager::buildJsonConfig()
{
	QJsonObject obj_main;//配置根(/)
	
	for (ConfigManager::ConfigElement n : configs)
	{
		obj_main.insert(n.key, n.value.toJsonValue());
	}

	QJsonDocument data_jsonDoc;
	data_jsonDoc.setObject(obj_main);
	data_jsonDoc.toJson(QJsonDocument::Compact);

	return data_jsonDoc;
}

std::vector<ConfigManager::ConfigElement> ConfigManager::parseJsonConfig(QJsonObject obj_root)
{
	std::vector<ConfigManager::ConfigElement> result;
	for (QString n : obj_root.keys())
	{
		ConfigManager::ConfigElement element;
		element.key = n;
		result.push_back(element);
	}
	//曾经在这里因为忘记加&导致出现Bug，整整找了一个小时≧ ﹏ ≦ ...迭代中修改值一定要记得加&！
	for (ConfigManager::ConfigElement &n : result)
	{
		n.value = obj_root.value(n.key).toVariant();
	}
	return result;
}

QVariant ConfigManager::getDefaultValue(QString key)
{
	for (ConfigManager::ConfigElement n : defaultConfigValues)
		if (n.key == key)
			return n.value;
	return NULL;
}

void ConfigManager::fillDefaultValue()
{
	for (ConfigElement &n : defaultConfigValues)
		setConfigValue(n.key, n.value);
}