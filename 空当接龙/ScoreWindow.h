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

	void getInfo();//接收数据

	void sendRequest(int time, QString name);//发送请求

	void testConnection();//测试链接

signals:
	void signalDataReady();//数据ready信号

	void signalTestConn(bool isConnecting);//测试链接信号

	void signalNextGame();//下一局按钮信号

private:
	QString readIPAddr();//获取IP地址


	Ui::ScoreWindow ui;
	QVector<User> userList;//存储排行信息
	QTcpSocket* clientSocket;//socket
	QTableView* viewRank;//排行榜
	QPushButton* updateGame;//下一局游戏
	
private slots:
	void showData();//显示排行

	void lookUpHost(QHostInfo info);//传数据

	void slotNextGameButton();//下一局游戏按钮槽函数
};
