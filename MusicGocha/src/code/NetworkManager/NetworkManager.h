#pragma once
#include <>
#include <QString>
#include <QStack>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct NetworkRequestPack
{
	QStack<QVariant> requestSender;
	QString url;
	QMap<QByteArray,QByteArray> header;
	enum
	{
		GET
	}method;
	QNetworkRequest::Priority priority;

};

class NetworkManager : QObject
{
	Q_OBJECT
public:
	NetworkManager();
	void addTask(NetworkRequestPack requestPack);

private:

	struct WorkingTask
	{
		QStack<QVariant> requestSender;
		QNetworkReply *reply;
	};

	QNetworkAccessManager networkAccess;
	QList<WorkingTask> workingTasks;
	void wakeupWorker();
	void taskFinished();
};