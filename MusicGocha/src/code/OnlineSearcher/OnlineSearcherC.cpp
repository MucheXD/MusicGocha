#include "OnlineSearcherC.h"
OnlineSearcherC::OnlineSearcherC(QWidget* parent)
{
	onlineSearcherU = new OnlineSearcherU;
	onlineSearcherU->setParent(parent);
}
void OnlineSearcherC::showWidget()
{
	onlineSearcherU->show();
}
bool OnlineSearcherC::tryDelete()
{
	//这里应该有一些判断是否有重要资源应保留的判断，若认为不该删除，返回false
	onlineSearcherU->hide();
	onlineSearcherU->deleteLater();
	return true;
}