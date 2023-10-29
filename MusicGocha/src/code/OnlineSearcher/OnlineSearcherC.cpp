#include "OnlineSearcherC.h"
OnlineSearcherC::OnlineSearcherC(QWidget* parent)
{
	widget_os = new OnlineSearcherW;
	widget_os->setParent(parent);
	connect(widget_os, &OnlineSearcherW::_startSearching, this, &OnlineSearcherC::startSearching);
}
void OnlineSearcherC::showWidget()
{
	widget_os->show();
}
bool OnlineSearcherC::tryDelete()
{
	//这里应该有一些判断是否有重要资源应保留的判断，若认为不该删除，返回false
	widget_os->hide();
	widget_os->deleteLater();
	return true;
}
QWidget* OnlineSearcherC::getWidgetPointer()
{
	return widget_os;
}
void OnlineSearcherC::startSearching()
{
	QStringList scriptList = emit _fetchConfigValue("work.online_search.script").toStringList();
	
}

OnlineSearchEngine::OnlineSearchEngine(QString scriptData)
{

}

std::vector<MusicInfo> OnlineSearchEngine::getSearchResult()
{

}