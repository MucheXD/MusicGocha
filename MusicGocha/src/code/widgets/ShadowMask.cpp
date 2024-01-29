#include "ShadowMask.h"

ShadowMask::ShadowMask()
{
}

void ShadowMask::showMask(QWidget* parent, uint8_t transparency)
{
	this->setStyleSheet(QString("background-color: rgba(0,0,0,%1);").arg(transparency));
	this->setParent(parent);
	this->setAutoFillBackground(true);
	this->setFixedSize(QSize(100000, 100000));
	this->show();
}

void ShadowMask::hideMask()
{
	this->hide();
}
