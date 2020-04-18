#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include "ui_EntryNameWindow.h"
#include "ScoreWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>

class EntryNameWindow : public QWidget
{
	Q_OBJECT

public:
	explicit EntryNameWindow(int time, QWidget* parent = nullptr);
	~EntryNameWindow();

private:
	Ui::EntryNameWindow ui;
	QLabel* labelTip;//提示信息
	QLineEdit* entryName;//输入框
	QPushButton* buttonSure;//确认按钮
	ScoreWindow* scoreRank;//排行榜窗口
	int timeofUser;//用户所用时间
	bool isOpenScore = false;//标志判断是否打开score窗口
 
signals:
	void signalNextGame();//下一关信号

private slots:
	void on_buttonSure_clicked();//退出键
	void slotNextGame();//下一关按键
};
