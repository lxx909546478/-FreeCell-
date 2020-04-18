#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif


#include "ui_ScoreWindow.h"
#include "user.h"
#include <QTableView>
#include <QWidget>
#include <QVector>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include <QStandardItemModel>
#include <QPushButton>
#include <QMessageBox>
#include <QHostInfo>
#include <QFile>
#include <QTextStream>
#include <algorithm>


class ScoreWindow : public QWidget
{
	Q_OBJECT

public:
	ScoreWindow( int score = 0, int time = 0, QString name = "", QWidget* parent = Q_NULLPTR);

	~ScoreWindow();

	void getInfo();//��������

	void sendRequest(int time, QString name);//��������

	void testConnection();//��������

signals:
	void signalDataReady();//����ready�ź�

	void signalTestConn(bool isConnecting);//���������ź�

	void signalNextGame();//��һ�ְ�ť�ź�

private:
	QString readIPAddr();//��ȡIP��ַ


	Ui::ScoreWindow ui;
	QVector<User> userList;//�洢������Ϣ
	QTcpSocket* clientSocket;//socket
	QTableView* viewRank;//���а�
	QPushButton* updateGame;//��һ����Ϸ
	
private slots:
	void showData();//��ʾ����

	void lookUpHost(QHostInfo info);//������

	void slotNextGameButton();//��һ����Ϸ��ť�ۺ���
};
