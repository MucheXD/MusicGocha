#include "DialogBox.h"

//弹出一个消息框
int DialogBox::popMessageBox(QWidget* parent, QString title, QString text
	, DialogBoxIconENUM icon
	, uint16_t btn, bool buttonAutoHighlighted, bool disableCloseButton
	, QString styleSheet)
{
	DialogBox dialogBox(parent);
	if (styleSheet != "")
		dialogBox.setStyleSheet(styleSheet);
	dialogBox.buildDialogBoxBody(title, text, icon);
	dialogBox.buildDialogBoxButton(btn, buttonAutoHighlighted, disableCloseButton);
	return dialogBox.exec();
}

DialogBox::DialogBox(QWidget* parent) : QDialog()//(parent)
{
	ui_db.setupUi(this);
	this->setWindowModality(Qt::WindowModal);
	this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);//禁用窗口大小手动调整，同时避免Windows下窗口自适应时报错
	this->setModal(true);
}

void DialogBox::buildDialogBoxBody(QString title, QString content, DialogBoxIconENUM icon, bool contentInMarkdown)
{
	ui_db.title_text->setText(title);
	setIcon(icon);
	//设置界面文字
	ui_db.title_text->setText(title);
	ui_db.content->setText(content);
	if (contentInMarkdown)
		ui_db.content->setTextFormat(Qt::MarkdownText);
	else
		ui_db.content->setTextFormat(Qt::PlainText);
}

void DialogBox::buildDialogBoxButton(uint16_t btn, bool buttonAutoHighlighted, bool disableCloseButton)
{
	//绘制控制区
	/*一种可行的判断高亮的过程：
	* 首先，高亮优先级由枚举项值的高位至低位逐渐降低
	* value*2-1的过程将value的最高位以下的全部位置1
	* current^(value*2-1)的结果如果大于value，则current有更高位为1，否则value的最高位1最高(即高亮)
	* E.G.btn ^ (DialogBoxButtonENUM::ok * 2 - 1)) <= DialogBoxButtonENUM::ok*/
//定义宏，方便对多个按钮执行类似的操作
#define CREATE_PREDEFINED_BUTTON_IFNEEDED(needed,name,enumId) \
if (needed & enumId)\
		connect(addButtonToInputs(createButton(name, enumId)),\
			&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
	if (btn != DialogBoxButtonENUM::btn_none)
	{
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "了解", DialogBoxButtonENUM::btn_ok);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "取消", DialogBoxButtonENUM::btn_cancel);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "确认", DialogBoxButtonENUM::btn_confirm);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "是", DialogBoxButtonENUM::btn_yes);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "否", DialogBoxButtonENUM::btn_no);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "重试", DialogBoxButtonENUM::btn_retry);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "终止", DialogBoxButtonENUM::btn_abort);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "使用默认", DialogBoxButtonENUM::btn_defalt);
		CREATE_PREDEFINED_BUTTON_IFNEEDED(btn, "自定义", DialogBoxButtonENUM::btn_custom);
		/*if ((btn & DialogBoxButtonENUM::btn_ok) == DialogBoxButtonENUM::btn_ok)
			connect(addButtonToInputs(createButton("了解", DialogBoxButtonENUM::btn_ok)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_cancel) == DialogBoxButtonENUM::btn_cancel)
			connect(addButtonToInputs(createButton("取消", DialogBoxButtonENUM::btn_cancel)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_defalt) == DialogBoxButtonENUM::btn_defalt)
			connect((createButton("置默认值", DialogBoxButtonENUM::btn_defalt)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_confirm) == DialogBoxButtonENUM::btn_confirm)
			connect(addButtonToInputs(createButton("确认", DialogBoxButtonENUM::btn_confirm)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_yes) == DialogBoxButtonENUM::btn_yes)
			connect(addButtonToInputs(createButton("是", DialogBoxButtonENUM::btn_yes)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_no) == DialogBoxButtonENUM::btn_no)
			connect(addButtonToInputs(createButton("否", DialogBoxButtonENUM::btn_no)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_retry) == DialogBoxButtonENUM::btn_retry)
			connect(addButtonToInputs(createButton("重试", DialogBoxButtonENUM::btn_retry)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
		if ((btn & DialogBoxButtonENUM::btn_abort) == DialogBoxButtonENUM::btn_abort)
			connect(addButtonToInputs(createButton("中止", DialogBoxButtonENUM::btn_abort)),
				&QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);*/
	}
	else if (disableCloseButton == false)
	{
		connect(addButtonToInputs(createButton("关闭", 0, buttonAutoHighlighted))
			, &QPushButton::clicked, this, &DialogBox::predefinedButtonClicked);
	}
	if (disableCloseButton == true)
		this->setWindowFlag(Qt::WindowCloseButtonHint, false);
}

void DialogBox::setIcon(DialogBoxIconENUM icon)
{
	ui_db.title_icon->setProperty("margin", 24);//图标空位
	if (icon == DialogBoxIconENUM::icon_none)
	{
		ui_db.title_icon->setProperty("icon", "none");
		ui_db.title_icon->setProperty("margin", 0);//取消图标空位
	}
	else if (icon == DialogBox::icon_information)
		ui_db.title_icon->setProperty("icon", "info_blue");
	else if (icon == DialogBox::icon_question)
		ui_db.title_icon->setProperty("icon", "question_blue");
	else if (icon == DialogBox::icon_warning)
		ui_db.title_icon->setProperty("icon", "warning_yellow");
	else if (icon == DialogBox::icon_warning_critical)
		ui_db.title_icon->setProperty("icon", "warning_red");
	else if (icon == DialogBox::icon_error)
		ui_db.title_icon->setProperty("icon", "error_red");
	else if (icon == DialogBox::icon_critical)
		ui_db.title_icon->setProperty("icon", "critical");
	else if (icon == DialogBox::icon_settings)
		ui_db.title_icon->setProperty("icon", "settings_blue");
}

void DialogBox::setIcon(QUrl iconUrl)
{
	ui_db.title_icon->setProperty("margin", 24);//图标空位
	ui_db.title_icon->setStyleSheet(QString("image: url(%1);").arg(iconUrl.toString()));
}

QPushButton* DialogBox::createButton(QString text, int returnId, bool isHighlighted)
{
	QPushButton* btn = new QPushButton;
	btn->setText(text);
	btn->setProperty("isHighlighted", isHighlighted);
	btn->setProperty("returnId", returnId);
	return btn;
}

QPushButton* DialogBox::addButtonToInputs(QPushButton* btn)
{
	ui_db.GL_inputs->addWidget(btn, ui_db.GL_inputs->rowCount() - 1, ui_db.GL_inputs->columnCount());//按钮全部添加到第一行的末尾，注意Count-1才是Index
	reLayoutInputs();//重新调整布局
	return btn;
}

void DialogBox::reLayoutInputs()
{
	//TODO 如果有输入框的情况下添加了按钮，则需要重新布局
}

void DialogBox::predefinedButtonClicked()
{
	QPushButton* p_senderButton = qobject_cast<QPushButton*>(sender());
	this->done(p_senderButton->property("returnId").toInt());
}

DialogBox::~DialogBox()
{
}