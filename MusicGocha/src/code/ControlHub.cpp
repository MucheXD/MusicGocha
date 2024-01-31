#include "ControlHub.h"

ControlHubCore::ControlHubCore()
{
	rootWindow = NULL;
	funcPages = {};
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
	connect(rootWindow, &RootWindow::_changeFuncPage, this, &ControlHubCore::changeFuncPage);
	//connect(rootWindow, &RootWindow::_fetchConfigValue, this, &ControlHubCore::getConfigValue);
	//connect(rootWindow, &RootWindow::_getNetworkReplyGET, this, &ControlHubCore::getNetworkReplyGET);
	rootWindow->setStyleSheet(globalStyleSheet);
	rootWindow->showWidget();
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

QNetworkReply* ControlHubCore::getNetworkReplyGET(QNetworkRequest& request)
{
	return networkAccessManager.get(request);
}

void ControlHubCore::addWorkToWorkCenter(std::vector<WorkRequest> requests)
{
	if (funcPages.workCenter == NULL)//如果业务页面未创建，先执行创建
		createFuncPage(FuncpageENUM::WorkCenter);
	funcPages.workCenter->addWork(requests);
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

void ControlHubCore::tryDeleteFuncPage(FuncpageENUM needDeletePage)
{
	if (needDeletePage==FuncpageENUM::OnlineSearcher && funcPages.onlineSearcher != NULL)
		if (funcPages.onlineSearcher->tryDelete())
			funcPages.onlineSearcher = NULL;
	if (needDeletePage == FuncpageENUM::WorkCenter && funcPages.workCenter != NULL)
		if (funcPages.workCenter->tryDelete())
			funcPages.workCenter = NULL;
	//TODO 由于页面未完成，此处缺失... 真的没有比这样枚举更好的方法了吗?
}

FuncPageABLE* ControlHubCore::createFuncPage(FuncpageENUM needCreatePage, bool enforced)
{
	
	if (needCreatePage == FuncpageENUM::OnlineSearcher)
	{
		if (funcPages.onlineSearcher == NULL || enforced)
			createFuncPage(funcPages.onlineSearcher);
		return funcPages.onlineSearcher;
	}
	if (needCreatePage == FuncpageENUM::WorkCenter)
	{
		if (funcPages.workCenter == NULL || enforced)
			createFuncPage(funcPages.workCenter);
		return funcPages.workCenter;
	}
}

void ControlHubCore::createFuncPage(OnlineSearcherC*& creation)
{
	creation = new OnlineSearcherC();
	connect(creation, &OnlineSearcherC::_fetchConfigValue,
		this, &ControlHubCore::getConfigValue);
	connect(creation, &OnlineSearcherC::_getNetworkReplyGET,
		this, &ControlHubCore::getNetworkReplyGET);
	connect(creation, &OnlineSearcherC::_addWorkToWorkCenter,
		this, &ControlHubCore::addWorkToWorkCenter);
}

void ControlHubCore::createFuncPage(WorkCenterC*& creation)
{
	creation = new WorkCenterC();
}

void ControlHubCore::changeFuncPage(FuncpageENUM newFuncPage)
{
	//向不是将调用的页面发送删除请求，是否执行内存释放由对应类自行判断
	//传入无效的页面会被tryDeleteFuncPage自动忽略，不用担心！
	for (FuncpageENUM nDelPage = (FuncpageENUM)(FuncpageENUM::NULLPAGE + 1); nDelPage < FuncpageENUM::ENDPAGE; nDelPage = (FuncpageENUM)(nDelPage + 1))
		tryDeleteFuncPage(nDelPage);

	//自动创建页面（或找到页面指针）并交给UI更新页面
	rootWindow->updateCurrentFuncPage(newFuncPage, createFuncPage(newFuncPage));
}
