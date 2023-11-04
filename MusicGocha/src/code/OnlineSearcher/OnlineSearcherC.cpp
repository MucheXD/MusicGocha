#include "OnlineSearcherC.h"
OnlineSearcherC::OnlineSearcherC(QWidget* parent)
{
	widget_os = new OnlineSearcherW;
	widget_os->setParent(parent);
	connect(widget_os, &OnlineSearcherW::_startSearching, this, &OnlineSearcherC::startSearching);

	
}
void OnlineSearcherC::showWidget()
{
	widget_os->show();
}
bool OnlineSearcherC::tryDelete()
{
	//这里应该有一些判断是否有重要资源应保留的判断，若认为不该删除，返回false
	widget_os->hide();
	widget_os->deleteLater();
	return true;
}
QWidget* OnlineSearcherC::getWidgetPointer()
{
	return widget_os;
}

void OnlineSearcherC::assembleSearchEngines()
{
	QStringList scriptList = emit _fetchConfigValue("work.online_search.scripts").toStringList();
	for (QString nLoadScriptFileName : scriptList)
	{
		QFile scriptFile;
		scriptFile.setFileName(nLoadScriptFileName);
		//异常检测并弹窗
		if (!scriptFile.open(QIODevice::ReadOnly))
		{
			DialogBox::popMessageBox((QWidget*)this, "加载搜索脚本时出错",
				QString("无法打开下列文件：%1").arg(nLoadScriptFileName),
				DialogBox::icon_error);
			continue;
		}
		if (scriptFile.size() >= 1024000)
		{
			DialogBox::popMessageBox((QWidget*)this, "加载搜索脚本时出错",
				QString("文件过大(大于100kB)：%1").arg(nLoadScriptFileName),
				DialogBox::icon_error);
			continue;
		}
		QByteArray scriptData = scriptFile.readAll();

		OnlineSearchEngine* engine = new OnlineSearchEngine(scriptData);


		

		//解析键值
		configs = parseJsonConfig(data_jsonDoc.object());
		return ConfigManager::no_error;
	}
}


void OnlineSearcherC::startSearching()
{
	//QStringList scriptList = emit _fetchConfigValue("work.online_search.script").toStringList();
	assembleSearchEngines();
}


