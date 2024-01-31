#include "OnlineSearcherC.h"

OnlineSearcherC::OnlineSearcherC()
{
	widget_os = new OnlineSearcherW(musicGroups, musicInfoDatabase);
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

void OnlineSearcherC::setWidgetParent(QWidget* parent)
{
	widget_os->setParent(parent);
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
		OnlineSearchEngine* engine = new OnlineSearchEngine;
		connect(engine, &OnlineSearchEngine::_getNetworkReplyGET,
			this, &OnlineSearcherC::pushRequest_getNetworkReplyGET);//桥接信号
		engine->loadScript(scriptData);
		engines.push_back(engine);
	}
}

void OnlineSearcherC::startSearching(QString keyword, QString methodId)
{
	if (engines.size() == 0)
		assembleSearchEngines();
	for (OnlineSearchEngine* engine : engines)
	{
		engine->startSearching(keyword, methodId);
		connect(engine, &OnlineSearchEngine::_finished, this, &OnlineSearcherC::engineFinished);
	}
}

void OnlineSearcherC::engineFinished(EngineTaskTarget targetType)
{
	OnlineSearchEngine* engine = qobject_cast<OnlineSearchEngine*>(sender());
	const QString currentEngineId = engine->getEngineId();
	const std::vector<MusicInfo> newMusicInfos = engine->takeResults();
	for (MusicInfo nAdding : newMusicInfos)
	{
		bool isJoined = false;
		for (MusicInfo &nFinding : musicInfoDatabase)//在数据库中查找是否已经存在这个MUI
		{
			if (nAdding.sourceId == nFinding.sourceId && nAdding.id == nFinding.id)//判据是源ID和源内音乐ID
			{
				nFinding = nAdding;
				isJoined = true;
				break;
			}
			if (!isJoined)//没有匹配，向数据库添加新项
				musicInfoDatabase.push_back(nAdding);
		}
	}
	GroupMusicInfos(newMusicInfos);

	if (targetType.isSearchTask())
		widget_os->updateUi();
}

void OnlineSearcherC::GroupMusicInfos()
{
	musicGroups.clear();
	GroupMusicInfos(musicInfoDatabase);
}

void OnlineSearcherC::GroupMusicInfos(std::vector<MusicInfo> const& newMusicInfo)
{
	for (MusicInfo nAnalysing : newMusicInfo)
	{
		bool isJoined = false;
		for (MusicGroup &nTrying : musicGroups)//在现有的组中查找可能合适的组
		{
			bool isFit = true;
			if (nAnalysing.title != nTrying.sharedTitle)
				isFit = false;
			if (nAnalysing.ablum.name != nTrying.sharedAblumName)
				isFit = false;
			if (isFit)
			{
				nTrying.includedMusics.push_back(&nAnalysing);
				isJoined = true;
			}
		}
		if (!isJoined)//没有找到合适的组，创建新组
		{
			MusicGroup newGroup;
			newGroup.sharedTitle = nAnalysing.title;
			newGroup.sharedAblumName = nAnalysing.ablum.name;
			newGroup.includedMusics.push_back(&nAnalysing);
			musicGroups.push_back(newGroup);
		}
	}
}

QNetworkReply* OnlineSearcherC::pushRequest_getNetworkReplyGET(QNetworkRequest& request)
{
	return emit _getNetworkReplyGET(request);
}
