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

#include "../basics.h"

struct AritstInfo
{
	QString id{};
	QString name{};
};
struct OnlineImageInfo
{
	QString url{};
	QByteArray image{};
};
struct AlbumInfo
{
	QString id{};
	QString name{};
	OnlineImageInfo cover;
};
struct DownloadInfo
{
	int32_t quality = -1;
	int32_t size = -1;
	QString url{};
};
struct MvInfo
{
	QString id{};
	QString title{};
	QString duration{};
	std::vector<DownloadInfo> downloads{};
};
struct LyricPhrase
{
	int32_t startTime = -1;
	int32_t endTime = -1;
	QString content{};
};
struct LyricInfo
{
	std::vector<LyricPhrase> oriLyric{};
	std::vector<LyricPhrase> transLyric{};
	std::vector<LyricPhrase> romLyric{};
};
struct MusicIndexs
{
	int32_t composite = -1;
	int32_t plays = -1;
	int32_t likes = -1;
	int32_t comments = -1;
};
struct MusicInfo
{
	QString id{};
	QString title{};
	QString transTitle{};
	QString subTitle{};
	QString duration{};
	QDateTime publishTime{};
	MusicIndexs indexs;
	AlbumInfo ablum;
	LyricInfo lyrics;
	OnlineImageInfo cover;
	MvInfo mv;
	std::vector<AritstInfo> artists;
	std::vector<DownloadInfo> downloads;

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
		QString runCollector{};
	};
	std::vector<SearchMethod> methods;

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
	std::vector<MusicInfo> takeResults();
private:
	OnlineSearcherScript script;
	QMap<QString, QVariant> globalData;
	std::vector<MusicInfo> innerDatabase;
	struct CollectorContinueInfo//收集器继续信息，用于在完成网络请求后继续特定collector
	{
		QString collectorId;
		QNetworkReply* networkReply{};
	};
	QMap<int32_t, CollectorContinueInfo> collectorContinues;//暂存正在等待网络请求响应的收集器,以唯一ID作为标识
	int32_t collectorContinuesIdCounter;//收集器暂存唯一ID，是一个累加器
	QSignalMapper collectorContinueSM;

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
	void _finished();//当全部收集器执行完毕时发射此信号，表明任务完成
};