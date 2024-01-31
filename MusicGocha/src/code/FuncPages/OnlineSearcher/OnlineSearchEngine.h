#pragma once

#include <QString>
#include <QStringList> 
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonvalue>
#include <QRegularExpression>
#include <QMap>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSignalMapper>

#include "../../basics.h"
#include "../../definitions/MusicInfoDefinition.h"

struct EngineTaskTarget
{
	enum TaskTargetTypeENUM
	{
		search_task,
		complete_task,
		download_task
	}type;
	bool isSearchTask() const {
		return (type == search_task);
	}
	bool isCompleteTask() const {
		return (type == complete_task);
	}
	bool isDownloadTask() const {
		return (type == download_task);
	}
};

enum CompleteTypeENUM
{
	complete_detailedInfo,
	complete_downloadInfo
};

struct OnlineSearcherScript
{
	QString id{};
	QString name{};
	QString description{};
	QString author{};
	QString target{};
	QString version{};
	struct
	{
		bool musicDownload = false;
		bool mvDownload = false;
		bool lyricDownload = false;
		bool multipleLyric = false;
	}ability;

	struct SearchMethod
	{
		QString id{};
		QString name{};
		QJsonArray runCollectors{};
	};
	std::vector<SearchMethod> methods;

	struct Completer
	{
		bool isBatchCompletion = false;
		QJsonArray runCollectors{};
	};
	Completer detailedInfoCompleter;
	Completer downloadInfoCompleter;

	struct Collector
	{
		QString id{};
		QJsonObject collectorScript;
	};
	std::vector<Collector> collectors;

	struct Parser
	{
		QString id{};
		QJsonObject translator{}; //如果buildTarget是MusicInfo的子信息，则忽略MusicInfo其它信息
	};
	std::vector<Parser> parsers;
};

class OnlineSearchEngine : public QObject
{
	Q_OBJECT
public:
	OnlineSearchEngine();
	void loadScript(QByteArray scriptData);
	void DEBUG_doParse(QByteArray data);
	void startSearching(QString keyword, QString methodId);
	void startCompleting(std::vector<MusicInfo> targets, CompleteTypeENUM completeType);
	//void startCompleteing(std::vector<MusicInfo> input);
	std::vector<MusicInfo> takeResults();
	QString getEngineId();
private:
	struct CollectorContinueInfo//收集器继续信息，用于在完成网络请求后继续特定collector
	{
		QString collectorId;
		EngineTaskTarget targetType;
		QNetworkReply* networkReply{};
	};

	OnlineSearcherScript script;
	QMap<QString, QVariant> globalData;
	std::vector<MusicInfo> innerDatabase;
	EngineTaskTarget currentRunningTaskTarget;
	QMap<int32_t, CollectorContinueInfo> collectorContinues;//暂存正在等待网络请求响应的收集器,以唯一ID作为标识
	int32_t collectorContinuesIdCounter;//收集器暂存唯一ID，是一个累加器
	QSignalMapper collectorContinueSM;

	QJsonArray unificationToJsonArray(QJsonValue const& jsonVal);

	void callCollector(QJsonObject callInfo, QMap<QString, QVariant> const& extraArguments = QMap<QString, QVariant>());
	void runCollector(QString collectorId, QMap<QString, QVariant> const& extraArguments = QMap<QString, QVariant>());
	void continueCollector(int32_t collectorContinueId);
	//通过collectorId找到其内部的collectorScript
	inline QJsonObject getCollectorScriptById(QString collectorId);
	QString replaceArgumentsToValue(QString oriString, QString replaceStartFlag = "[", QString replaceEndFlag = "]", QString arraySeparator = ",", QMap<QString, QVariant> const& extraArguments = QMap<QString, QVariant>());


	//获取Json复合数据data中位于path路径的值，不支持Array嵌套
	QJsonValue getJsonValueByPath(QJsonObject const& data, QString path);
	//获取通过translator对应的实际数据的值，keyInTranslator对应的内容只能是String
	inline QJsonValue getJsonValueWithTranslator(const QJsonObject& input, const QJsonObject& translator, QString keyInTranslator);
	//完成解析工作：结果为单个
	template<typename T> void runParser(QJsonObject const& input, T& output, QJsonObject callInfo);
	//完成解析工作：结果为vector
	template<typename T> void runParser(const QJsonObject& input, std::vector<T>& output, QJsonObject callInfo);
	//通过parserId找到其内部的translator
	QJsonObject getTranslatorByParserId(QString parserId);
	//输入原始Json数据和translator，输出格式化后的Struct
	void fillStructFromJson(QJsonObject const& input, MusicInfo& output, QJsonObject const& translator);
	void fillStructFromJson(QJsonObject const& input, AlbumInfo& output, QJsonObject const& translator);
	void fillStructFromJson(QJsonObject const& input, MvInfo& output, QJsonObject const& translator);
	void fillStructFromJson(QJsonObject const& input, AritstInfo& output, QJsonObject const& translator);
	void fillStructFromJson(QJsonObject const& input, LyricInfo& output, QJsonObject const& translator);
	void fillStructFromJson(QJsonObject const& input, DownloadInfo& output, QJsonObject const& translator);

signals:
	QNetworkReply* _getNetworkReplyGET(QNetworkRequest& request);
	void _finished(EngineTaskTarget targetType);//当全部收集器执行完毕时发射此信号，表明任务完成
};