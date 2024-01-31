#pragma once

#include "../../definitions/WorkRequestDefinition.h"
#include "../FuncPageABLE.h"

class WorkCenterC : public FuncPageABLE
{
	Q_OBJECT
public:
	WorkCenterC();

	void showWidget();
	bool tryDelete();
	void setWidgetParent(QWidget* parent);
	QWidget* getWidgetPointer();

	void addWork(std::vector<WorkRequest> requests);

private:
	struct WorkGroup
	{
		QString sharedWorkId{};
		QString workDisplayName{};
		std::vector<WorkRequest> includedRequests;
	};
	std::vector<WorkGroup> workGroups;
};
