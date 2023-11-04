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
struct MVINFO
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
	QString title{};
	QString transTitle{};
	QString subTitle{};
	QString id{};
	int32_t duration = -1;
	QDateTime publishTime{};
	MusicIndexs indexs;
	AlbumInfo ablum;
	LyricInfo lyrics;
	OnlineImageInfo cover;
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
		QString script_id{};
	};
	std::vector<SearchMethod> searchingMethods;
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
	
};