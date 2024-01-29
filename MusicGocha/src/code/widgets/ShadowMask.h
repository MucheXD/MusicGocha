#pragma once

#include <QWidget>
#include <QGridLayout>

class ShadowMask : QWidget
{
	Q_OBJECT
public:
	ShadowMask();
	void showMask(QWidget* parent, uint8_t transparency = 90);
	void hideMask();
private:
};