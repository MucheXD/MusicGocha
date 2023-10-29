#pragma once
#include <QDatetime>
#include "OnlineSearcherW.h"
#include "../widgets/DialogBox.h"

struct OnlineSearcherScript
{
	QString id{};
	QString name{};
	QString description{};
	QString author{};
	QString target{};
	QString version{};
	QDateTime createTime{};
	struct
	{
		bool musicDownload = false;
		bool mvDownload = false;
		bool lyricDownload = false;
		bool multipleLyric = false;
	}ability;
	struct SearchingMethod
	{
		QString id{};
		QString name{};
		QString script{};
	};
	std::vector<SearchingMethod> searchingMethods;
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

class OnlineSearcherC : public QObject
{
	Q_OBJECT
public:
	OnlineSearcherC(QWidget* parent);
	void showWidget();
	bool tryDelete();
	void startSearching();
	QWidget* getWidgetPointer();
private:
	OnlineSearcherW* widget_os;
	std::vector<OnlineSearcherScript> searcherScripts;
signals:
	QVariant _fetchConfigValue(QString key);
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
struct AritstInfo
{
	QString id{};
	QString name{};
};
struct AlbumInfo
{
	QString id{};
	QString name{};
	OnlineImageInfo cover;
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
struct OnlineImageInfo
{
	QString url{};
	QByteArray data{};
};
struct MusicIndexs
{
	int32_t composite = -1;
	int32_t plays = -1;
	int32_t likes = -1;
	int32_t comments = -1;
};
struct DownloadInfo
{
	int32_t quality = -1;
	int32_t size = -1;
	QString url{};
};

class OnlineSearchEngine
{
public:
	OnlineSearchEngine(QString scriptData);
	std::vector<MusicInfo> getSearchResult();
};