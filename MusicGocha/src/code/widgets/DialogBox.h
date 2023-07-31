#pragma once
#include <QPushButton>
#include <QStyle>
#include "ui_DialogBox.h"

class DialogBox : public QDialog
{
public:

	//可用的图标
	enum DialogBoxIconENUM
	{
		icon_none,
		icon_information,
		icon_question,
		icon_warning,
		icon_warning_critical,
		icon_error,
		icon_critical,
		icon_settings
	};
	
	//预定义的按钮
	enum DialogBoxButtonENUM : uint16_t
	{
		btn_none = 0,
		btn_ok = 1,
		btn_cancel = 2,
		btn_confirm = 4,
		btn_no = 8,
		btn_yes = 16,
		btn_retry = 32,
		btn_abort = 64,
		btn_continue = 128,
		btn_defalt = 256,
		btn_custom = 512,
	};

	DialogBox(QWidget* parent);
	~DialogBox();
	static int popMessageBox(QWidget* parent, QString title, QString text
		, DialogBoxIconENUM icon = DialogBoxIconENUM::icon_none
		, uint16_t btn = DialogBoxButtonENUM::btn_none
		, bool buttonAutoHighlighted = true, bool disableCloseButton = false
		, QString styleSheet = "");
	
private:
	void buildDialogBoxBody(QString title, QString content
		, DialogBoxIconENUM icon = DialogBoxIconENUM::icon_none, bool contentInMarkdown = false);
	void buildDialogBoxButton(uint16_t btn = DialogBoxButtonENUM::btn_none
		, bool buttonAutoHighlighted = true, bool disableCloseButton = false);
	void setIcon(DialogBoxIconENUM icon);//设置默认的图标
	void setIcon(QUrl iconUrl);//设置资源文件中的图标
	QPushButton* createButton(QString text, int returnId = -1, bool isHighlighted = false);
	QPushButton* addButtonToInputs(QPushButton* btn);
	void reLayoutInputs();
	void predefinedButtonClicked();
	Ui::DialogBoxU ui_db;
};