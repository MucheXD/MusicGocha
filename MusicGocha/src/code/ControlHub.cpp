#include "ControlHub.h"

ControlHubCore::ControlHubCore()
{
	rootWindow = NULL;
}

bool ControlHubCore::launchApp()
{
	if (!loadStyleSheet())
	{
		if (DialogBox::popMessageBox(NULL,
			"无法载入样式表",
			"无法加载样式表 configs/style.qss\n"
			"如果你选择继续运行，程序界面可能无法正确显示。要继续运行程序吗？",
			DialogBox::icon_warning_critical,
			DialogBox::btn_no | DialogBox::btn_yes)
			== DialogBox::btn_no)
			exit(-1);
	}
	ConfigManager::ErrorList configLoadingError = configManager.loadConfig();
	if (configLoadingError!=ConfigManager::no_error)
	{
		if (configLoadingError == ConfigManager::file_not_exist)
		{
			if (DialogBox::popMessageBox(NULL,
				"欢迎使用 MusicGocha!",
				"你似乎是第一次使用此 Musicgocha 副本，无论如何，欢迎使用ヾ(≧▽≦*)o\n"
				"在开始之前，你希望 MusicGocha 如何进行初始设置？\n"
				"你想要自定义所有选项，还是让 MusicGocha 使用默认值替你决定？",
				DialogBox::icon_settings,
				DialogBox::btn_custom | DialogBox::btn_defalt,
				false, false, globalStyleSheet)
				== DialogBox::btn_defalt)
			{
				configManager.fillDefaultValue();
			}
			if (!configManager.saveConfig(true))
				DialogBox::popMessageBox(NULL,
					"无法写入配置文件",
					"我们无法创建configs/config.mtd\n",
					DialogBox::icon_error,
					DialogBox::btn_ok,
					false, false, globalStyleSheet);	
		}
	}
	//启动RootWindow
	rootWindow = new RootWindow;
	connect(rootWindow, &RootWindow::_fetchConfigValue, this, &ControlHubCore::getConfigValue);
	rootWindow->setStyleSheet(globalStyleSheet);
	return true;
}

bool ControlHubCore::loadStyleSheet()
{
	QFile file;
	file.setFileName("configs/style.qss");
	if (!file.open(QIODevice::ReadOnly) || file.size() >= 0x100000)//大小大于1MB或打开失败均视作失败
		return false;
	globalStyleSheet = file.readAll();
	file.close();
	return true;
}

QVariant ControlHubCore::getConfigValue(QString key)
{
	return configManager.getConfigValue(key);
}

void ControlHubCore::setConfigValue(QString key, QVariant value)
{
	configManager.setConfigValue(key, value);
}

ControlHubCore::~ControlHubCore()
{
	while (true)
	{
		if (configManager.saveConfig())
			break;
		else
			if (DialogBox::popMessageBox(NULL,
				"无法写入配置文件",
				"请检查configs/config.mtd是否存在且可访问\n"
				"如果你选择终止，程序会正常退出，但对配置做出的修改将不会保存\n",
				DialogBox::icon_error,
				DialogBox::btn_abort | DialogBox::btn_retry,
				false, false, globalStyleSheet)
				== DialogBox::btn_abort)
				break;
	}
}