#pragma once
#include <QString>

struct DownloadConfig
{
	QString enforcedSourceId{};
	bool downloadMusic{};
	uint16_t qualityPreference; //使用 0 规定最小音质，65536规定最大音质
};