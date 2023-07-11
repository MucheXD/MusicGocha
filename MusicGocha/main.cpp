#include <QApplication>
#include "ControlHub.h"

int main(int argc, char *argv[])
{
	QApplication qApplication(argc, argv);
	ControlHubCore controlHubCore;
	controlHubCore.launchApp();
	return qApplication.exec();
}