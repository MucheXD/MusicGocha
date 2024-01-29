#pragma once
#include <QWidget>
#include "../definitions/MusicInfoDefinition.h"
#include "ui_SearchResultDetailedInfoU.h"

class SearchResultDetailedInfoW : public QWidget
{
	Q_OBJECT
public:
	SearchResultDetailedInfoW(QWidget* parent,const MusicGroup& setData);
	void showWidget();
	void updateGeometryFollowingParent();
	void updateUi();
	void closeWidget();
private:
	Ui::SearchResultDetailedInfoU ui_di;
	QWidget* parentWidget;
	const MusicGroup& currentData;
signals:
	void _widgetClosed();
};