#pragma once
#include <QPushButton>
#include <QStyle>
#include "ui_DialogBox.h"

class DialogBox : public QDialog
{
public:

	enum DialogBoxIconENUM
	{
		icon_none = 0b00000000,
		icon_information = 0b00010000,
		icon_question = 0b00010001,
		icon_warning = 0b00100000,
		icon_warning_critical = 0b00100001,
		icon_error = 0b00110000,
		icon_critical = 0b01000000
	};
	
	//预定义的按钮
	enum DialogBoxButtonENUM : uint16_t
	{
		btn_none = 0b00000000,
		btn_ok = 0b00000001,
		btn_cancel = 0b00000010,
		btn_defalt = 0b00000100,
		btn_confirm = 0b00001000,
		btn_no = 0b00010000,
		btn_yes = 0b00100000,
		btn_retry = 0b01000000,
		btn_abort = 0b10000000
	};

	DialogBox(QWidget* parent);
	~DialogBox();
	void showMessageBox(QString title, QString text
		, DialogBoxIconENUM icon = DialogBoxIconENUM::icon_none
		, uint16_t btn = DialogBoxButtonENUM::btn_none
		, bool buttonAutoHighlighted = true, bool disableCloseButton = false);
	static int popMessageBox(QWidget* parent, QString title, QString text
		, DialogBoxIconENUM icon = DialogBoxIconENUM::icon_none
		, uint16_t btn = DialogBoxButtonENUM::btn_none
		, bool buttonAutoHighlighted = true, bool disableCloseButton = false);
	
private:
	void buildDialogBoxBody(QString title, QString text
		, DialogBoxIconENUM icon = DialogBoxIconENUM::icon_none);
	void buildDialogBoxButton(uint16_t btn = DialogBoxButtonENUM::btn_none
		, bool buttonAutoHighlighted = true, bool disableCloseButton = false);
	QPushButton* createButton(QString text, int returnId = -1, bool isHighlighted = false);
	QPushButton* addButtonToInputs(QPushButton* btn);
	void reLayoutInputs();
	void predefinedButtonClicked();
	Ui::DialogBoxU ui_db;
};