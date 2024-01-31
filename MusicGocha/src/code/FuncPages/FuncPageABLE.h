#pragma once
#include <QObject>

class FuncPageABLE : public QObject
{
public:
	virtual void showWidget() = NULL;
	virtual bool tryDelete() = NULL;
	virtual void setWidgetParent(QWidget* parent) = NULL;
	virtual QWidget* getWidgetPointer() = NULL;
};