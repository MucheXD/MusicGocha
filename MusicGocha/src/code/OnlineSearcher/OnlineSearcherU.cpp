#include "OnlineSearcherU.h"

OnlineSearcherU::OnlineSearcherU(QWidget* parent)
{
	ui_os.setupUi(this);

	QTimer* testtimer = new QTimer;
	connect(testtimer, &QTimer::timeout, this, &OnlineSearcherU::testifdeleted);
	testtimer->start(1000);


	this->setParent(parent);
}

bool OnlineSearcherU::tryDelete()
{
	//这里应该有一些判断是否有重要资源应保留的判断，若认为不该删除，返回false
	this->hide();
	this->deleteLater();
	qDebug() << "Deleted?";
	return true;
}

void OnlineSearcherU::testifdeleted()
{
	qDebug() << "Not deleted yet";
}