#include "OnlineSearcherC.h"

OnlineSearcherC::OnlineSearcherC()
{
	widget_os = new OnlineSearcherW(musicGroups, musicInfoDatabase);
	completerContinuesIdCounter = 0;
	connect(widget_os, &OnlineSearcherW::_startSearching,
		this, &OnlineSearcherC::startSearching);	
	connect(widget_os, &OnlineSearcherW::_callDownload,
		this, &OnlineSearcherC::startContentDownload);
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
		connect(engine, &OnlineSearchEngine::_finished,
			this, &OnlineSearcherC::engineFinished);
		engine->loadScript(scriptData);
		engines.push_back(engine);
	}
	if (engines.size() == 0)
		throw "NO_ENGINE_ENABLE";
}

void OnlineSearcherC::startSearching(QString keyword, QString methodId)
{
	if (engines.size() == 0)
		assembleSearchEngines();
	for (OnlineSearchEngine* engine : engines)
	{
		engine->startSearching(keyword, methodId);
	}
}

void OnlineSearcherC::startCompletion(std::vector<MusicInfo> needComplete,CompleteTypeENUM completeType, int32_t continueInfoId)
{
	if (engines.size() == 0)
		assembleSearchEngines();
	int32_t requestSendedNum = 0;
	for (OnlineSearchEngine* engine : engines)
	{
		EngineTaskTarget engineTaskTarget;
		engineTaskTarget.aim = continueInfoId;
		engineTaskTarget.type = EngineTaskTarget::complete_task;
		std::vector<MusicInfo> completableInfo;
		for (auto i = 0; i < needComplete.size(); i++)
		{
			if (engine->getEngineId() == needComplete.at(i).sourceId)
			{
				completableInfo.push_back(needComplete.at(i));
				//TODO 匹配完成的已发送项应该删除
				requestSendedNum += 1;
			}
		}
		engine->startCompletion(completableInfo, completeType, engineTaskTarget);	
	}
	if (requestSendedNum < needComplete.size())
		throw "NOT_ALL_SENDED";//注意，上个TODO完成后需要同步修改此处逻辑
}

void OnlineSearcherC::continueCompletion(int32_t continueInfoId)
{
	auto currentContinueInfoItera = completerContinueInfos.find(continueInfoId);
	CompleterContinueInfo currentContinueInfo;
	if (currentContinueInfoItera != completerContinueInfos.end()) 
	{
		currentContinueInfo = currentContinueInfoItera.value();
	}
	else 
	{
		throw "WOW!NOCONTINUEINFO!";
	}
	//WORKING 合并新的信息并交付下载
	if (currentContinueInfo.redirect == CompleterContinueInfo::send_to_work)
	{
		downloadContents();
	}
}

void OnlineSearcherC::engineFinished(EngineTaskTarget taskTarget)
{
	OnlineSearchEngine* engine = qobject_cast<OnlineSearchEngine*>(sender());
	const QString currentEngineId = engine->getEngineId();
	const std::vector<MusicInfo> newMusicInfos = engine->takeResults();
	mergeMusicInfoSet(musicInfoDatabase, newMusicInfos);
	GroupMusicInfos(newMusicInfos);

	if (taskTarget.isSearchTask())
		widget_os->updateUi();
	if (taskTarget.isCompleteTask())
		continueCompletion(taskTarget.aim.toInt());
}

void OnlineSearcherC::mergeMusicInfoSet(std::vector<MusicInfo> &mainSet, std::vector<MusicInfo> &newSet)
{
	for (MusicInfo nAdding : newSet)
	{
		bool isJoined = false;
		for (MusicInfo& nFinding : mainSet)//在数据库中查找是否已经存在这个MUI
		{
			if (nAdding.sourceId == nFinding.sourceId && nAdding.id == nFinding.id)//判据是源ID和源内音乐ID
			{
				nFinding = nAdding;
				isJoined = true;
				break;
			}
			if (!isJoined)//没有匹配，向数据库添加新项
				mainSet.push_back(nAdding);
		}
	}
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

void OnlineSearcherC::prepareContentsDownload(MusicGroup& target, int32_t workConfigIndex)
{
	WorkRequest workRequest;//向workCenter发送的注册数据
	workRequest.workId = QString("OS.%1-%2")
		.arg(target.sharedTitle.toUtf8().toBase64(), 8, '0')
		.arg(QString::number(clock(), 16), 4, '0');//生成工作ID
	workRequest.workType = WorkRequest::work_register;
	workRequest.workInfo.insert("title", "⬇️ | "+target.sharedTitle);
	_addWorkToWorkCenter(workRequest); //向workCenter 注册任务

	//检查完整级别在 detailed 以下的MUI并执行补全
	std::vector<MusicInfo> needCompleteElement;
	for (auto &currentCheck : target.includedMusics)
	{
		if (currentCheck->infoIntegrality == MusicInfo::integrality_basic) 
			needCompleteElement.push_back(*currentCheck);
	}
	if (needCompleteElement.empty())
	{
		
	}
	else
	{
		CompleterContinueInfo continueInfo;//用于稍后继续收集器的数据
		continueInfo.extraInfo.insert("workId", workRequest.workId);
		continueInfo.redirect = CompleterContinueInfo::send_to_work;
		for (MusicInfo* nAdd : target.includedMusics)
			continueInfo.originalMusicInfos.push_back(*nAdd);//加入原始MUI(将来可能需要它们参与下载决策)
		int32_t currentId = completerContinuesIdCounter();
		startCompletion(needCompleteElement, CompleteTypeENUM::complete_detailed, currentId);
		completerContinueInfos.insert(currentId,continueInfo);//保存继续信息到类
	}
}


//void OnlineSearcherC::startContentDownload(MusicGroup& target, int32_t workConfigIndex)
//{
//	std::vector<MusicInfo> needComplete;
//	CompleterContinueInfo compContinueInfo;	//继续信息，以准备completer返回时的操作
//
//	for (MusicInfo* nCheck : target.includedMusics)
//	{
//		compContinueInfo.musicInfos.push_back(*nCheck);//即使无需补全的也需要加入
//		if (!nCheck->infoIntegrality.downloadInfo)
//			needComplete.push_back(*nCheck);
//	}
//
//	WorkRequest workRequest;
//	workRequest.workType = WorkRequest::work_register;
//	workRequest.workId = QString("OS.%1").arg(target.sharedTitle.toUtf8().toBase64());	//TODO 优化ID生成以避免同名歌曲无法下载
//	workRequest.workInfo = workConfigs.at(workConfigIndex);
//	emit _addWorkToWorkCenter(workRequest);
//	compContinueInfo.aim = workRequest.workId;
//	compContinueInfo.redirect = compContinueInfo.send_to_work;
//	compContinueInfo.taskId = completerContinuesIdCounter;
//	completerContinuesIdCounter += 1;
//
//	//启动补全器
//	startCompletion(needComplete, CompleteTypeENUM::complete_downloadInfo, compContinueInfo.taskId);
//}

QNetworkReply* OnlineSearcherC::pushRequest_getNetworkReplyGET(QNetworkRequest& request)
{
	return emit _getNetworkReplyGET(request);
}
