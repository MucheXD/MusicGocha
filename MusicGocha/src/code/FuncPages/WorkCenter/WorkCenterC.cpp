#include "WorkCenterC.h"

WorkCenterC::WorkCenterC()
{
}

bool WorkCenterC::tryDelete()
{
	return false;
}

void WorkCenterC::showWidget()
{
}

void WorkCenterC::setWidgetParent(QWidget* parent)
{
}

QWidget* WorkCenterC::getWidgetPointer()
{
	return nullptr;
}

void WorkCenterC::addWork(std::vector<WorkRequest> requests)
{
	_beep(1000,1000);
}
