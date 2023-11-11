#pragma once

#include <QString>
#include <QStringList> 
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonvalue>

struct AritstInfo
{
	QString id{};
	QString name{};
};
struct OnlineImageInfo
{
	QString url{};
	QByteArray data{};
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
	int32_t duration = -1;
	int32_t size = -1;
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

enum SEARCHENGINERET
{
	no_error = 0,
	script_illegaljson,
	
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
		QString scriptId{};
	};
	std::vector<SearchMethod> searchingMethods;

	struct Collector
	{
		QString id{};
		QString url{};
		struct
		{
			QString keyword{};
			QString pageSize{};
			QString page{};
			QString method{};
		}arguments;
		struct
		{
			QString successCheckKey = {};
			QString successFlag = {};
			QString errorMsg = {};
		}responseCheck;
		struct
		{
			QString parserId = {};
			QString key = {};
		}content;
	};
	std::vector<Collector> collectors;

	struct Parser
	{
		QString id{};
		QString buildTarget{};
		MusicInfo data{}; //如果buildTarget是MusicInfo的子信息，则忽略MusicInfo其它信息
	};
	std::vector<Parser> parsers;
	struct
	{
		QString title;
		QString duration;
		QString indexs;
		QString ablum;
		QString lyrics_info;
		QString lyrics;
		QString cover_info;
		QString cover;
		QString download_info;
	};
};

class OnlineSearchEngine
{
public:
	OnlineSearchEngine();
	SEARCHENGINERET loadScript(QByteArray scriptData);
	std::vector<MusicInfo> getSearchResult();
private:
	OnlineSearcherScript script;
	bool fillStructFromJson(QJsonObject jsonData, MusicInfo* target);
	bool fillStructFromJson(QJsonObject jsonData, AlbumInfo* target);
	bool fillStructFromJson(QJsonObject jsonData, MvInfo* target);
	bool fillStructFromJson(QJsonObject jsonData, AritstInfo* target);
	bool fillStructFromJson(QJsonObject jsonData, LyricInfo* target);
	bool fillStructFromJson(QJsonObject jsonData, DownloadInfo* target);
	void parseNameonlyArtistsName(QString rawText, QString separator, std::vector<AritstInfo>* target);
};