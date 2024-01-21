#pragma once

#include <QString>
#include <QByteArray>
#include <vector>
#include <QDateTime>

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
	QString sourceId;

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

struct MusicInfoRef
{
	QString engineId{};
	QString musicId{};
};

struct MusicGroup
{
	QString sharedTitle;
	QString sharedAblumName;
	QStringList sharedArtistsName;
	std::vector<MusicInfo*> includedMusics;
};