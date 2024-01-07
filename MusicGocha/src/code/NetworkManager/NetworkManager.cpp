#include "NetworkManager.h"

NetworkManager::NetworkManager()
{
	
}

void NetworkManager::addTask(NetworkRequestPack requestPack)
{
	QNetworkRequest networkReq;
	//装配请求包
	networkReq.setUrl(QUrl(requestPack.url));
	QMap<QByteArray, QByteArray>::iterator headerMapIter;
	for (headerMapIter = requestPack.header.begin(); headerMapIter != requestPack.header.end(); ++headerMapIter)
		networkReq.setRawHeader(headerMapIter.key(),headerMapIter.value());
	networkReq.setPriority(requestPack.priority);
	//发送请求
	WorkingTask currenTask{};
	if (requestPack.method == NetworkRequestPack::GET)
		currenTask.reply = networkAccess.get(networkReq);
	else
		throw "NERROR";
	connect(currenTask.reply, QNetworkReply::finished, this, taskFinished);
	workingTasks.append(currenTask);
}
