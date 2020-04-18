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
	QLabel* labelTip;//��ʾ��Ϣ
	QLineEdit* entryName;//�����
	QPushButton* buttonSure;//ȷ�ϰ�ť
	ScoreWindow* scoreRank;//���а񴰿�
	int timeofUser;//�û�����ʱ��
	bool isOpenScore = false;//��־�ж��Ƿ��score����
 
signals:
	void signalNextGame();//��һ���ź�

private slots:
	void on_buttonSure_clicked();//�˳���
	void slotNextGame();//��һ�ذ���
};
