#pragma once
#include <QString>
#include <QJsonObject>

struct WorkRequest
{
	QString workId;//相同的ID将在处理时合并为一个大任务
	enum {
		work_register,
		work_download
	}workType;
	QJsonObject workInfo;
};