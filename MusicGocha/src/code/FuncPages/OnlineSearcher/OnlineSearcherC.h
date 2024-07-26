#pragma once
#include <QDatetime>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "OnlineSearcherW.h"
#include "../../widgets/DialogBox.h"
#include "OnlineSearchEngine.h"
#include "../FuncPageABLE.h"
#include "../../definitions/WorkRequestDefinition.h"

class OnlineSearcherC : public FuncPageABLE
{
	Q_OBJECT
public:
	OnlineSearcherC();

	void showWidget();
	bool tryDelete();
	void setWidgetParent(QWidget* parent);
	QWidget* getWidgetPointer();

	void assembleSearchEngines();
	
private:

	struct CompleterContinueInfo
	{
		QJsonObject extraInfo;
		std::vector<MusicInfo&> originalMusicInfosRef;
		enum
		{
			send_to_work,
			merge_to_database
		}redirect;
	};


	OnlineSearcherW* widget_os;
	//std::vector<OnlineSearcherScript> searcherScripts;
	std::vector<OnlineSearchEngine*> engines;
	std::vector<MusicInfo> musicInfoDatabase;
	std::vector<MusicGroup> musicGroups;
	std::vector<QJsonObject> workConfigs;
	QMap<int32_t, CompleterContinueInfo> completerContinueInfos;
	IncrementInt32 completerContinuesIdCounter; 

	void startSearching(QString keyword, QString methodId);
	void startCompletion(std::vector<MusicInfo> needComplete, CompleteTypeENUM completeType, int32_t continueInfoId);
	void continueCompletion(int32_t continueInfoId);
	void engineFinished(EngineTaskTarget targetType);
	void mergeMusicInfoSet(std::vector<MusicInfo> &mainSet, const std::vector<MusicInfo> &newSet);
	//根据MUI数据库重新构建分组
	void GroupMusicInfos();
	//向分组内添加新的MUI。注意：为了避免野指针，删除MUI时不得使用此重载。
	void GroupMusicInfos(std::vector<MusicInfo> const& newMusicInfo);

	void prepareContentsDownload(MusicGroup& target, int32_t workConfigIndex);

	void downloadContents(std::vector<MusicInfo&> musicInfos, QString workId, int32_t workConfigIndex);

	//给定一个MUG(在UI上作为一个单元)，检查下载信息是否完整以及向WorkCenter提供任务注册
	//void sendContentsDownload(std::vector<MusicInfo&> musicInfos, int32_t workConfigIndex);
	//补全器完成后，调用该函数，由该函数负责装配最终任务信息并提交给WorkCenter
	//void contentDownloadContinue(int32_t completerContinueInfoIndex);
	QNetworkReply* pushRequest_getNetworkReplyGET(QNetworkRequest& request);
signals:
	QVariant _fetchConfigValue(QString key);
	QNetworkReply* _getNetworkReplyGET(QNetworkRequest& request);
	void _addWorkToWorkCenter(WorkRequest request);
};


