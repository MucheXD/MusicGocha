#include "OnlineSearcherC.h"
OnlineSearcherC::OnlineSearcherC(QWidget* parent)
{
	widget_os = new OnlineSearcherW;
	widget_os->setParent(parent);
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