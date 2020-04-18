#include "ScoreWindow.h"

ScoreWindow::ScoreWindow(int score /* = 0 */, int time /* = 0 */, QString name /* = "" */, QWidget* parent /* = Q_NULLPTR */)
	: QWidget(parent)
{
	ui.setupUi(this);
	setGeometry(300, 100, 762, 609);

	QPalette palColor;
	palColor.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/Resources/scoreWindow.png")));
	setPalette(palColor);//设置底色

	QString IPAddr = readIPAddr();//IP地址

	clientSocket = new QTcpSocket(this);//建立TCP链接
	clientSocket->connectToHost(IPAddr, 9999);

	viewRank = new QTableView(this);//排行榜
	viewRank->setGeometry(150, 225, 400, 350);
	viewRank->setStyleSheet("background-color:#FFFFFF;");

	updateGame = new QPushButton(this);//下一局按键
	updateGame->setGeometry(580, 480, 150, 40);
	updateGame->setText("下一局");
	updateGame->setStyleSheet("color:#888888;");
	connect(updateGame, &QPushButton::clicked, this, &ScoreWindow::slotNextGameButton);

	//setAttribute(Qt::WA_DeleteOnClose, 1);

	testConnection();
	//断连显示
	connect(clientSocket, &QTcpSocket::disconnected, []() {
		QMessageBox::critical(NULL, "ERROR", "连接已断开");
		});
	//处理网络检测事件
	connect(this, &ScoreWindow::signalTestConn, [=](bool isConnecting)
		{
			if (isConnecting)
			{
				//接收到消息后，将数据存入本地数据结构用户信息数组中
				connect(clientSocket, &QTcpSocket::readyRead, this, &ScoreWindow::getInfo);
				//用户信息数组准备好后，开始显示
				connect(this, &ScoreWindow::signalDataReady, this, &ScoreWindow::showData);
				//向服务器发送请求信息
				sendRequest(time, name);
			}
			else
			{
				QMessageBox::critical(NULL, "ERROR", "网络未连接");
			}
		});
}

void ScoreWindow::slotNextGameButton()//下一关
{
	emit signalNextGame();
	this->close();
}

QString ScoreWindow::readIPAddr()//读文件从而获取IP地址
{
	QFile f(":/txt/IPAddr");
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))//打开指定文件
	{
		QMessageBox::about(NULL, "文件", "IPAddr.txt打开失败");
		return "";
	}
	QTextStream txtInput(&f);
	QString IPAddr = txtInput.readLine();  //读取数据
	f.close();
	return IPAddr;
}

void ScoreWindow::lookUpHost(QHostInfo info)//lookupHost的处理函数，将其函数指针传入
{
	if (info.error()== QHostInfo::NoError)
	{
		qDebug() << "网络正常" << endl;
		emit signalTestConn(true);
	}
	else
	{
		qDebug() << "网络异常" << endl;
		emit signalTestConn(false);
	}
}

void ScoreWindow::testConnection()//检测网络状况
{
	QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookUpHost(QHostInfo)));
}


void ScoreWindow::showData()//显示排行榜信息
{
	std::sort(userList.begin(), userList.end(), [](User lhs, User rhs) { return lhs.getTime() < rhs.getTime(); });
	QStandardItemModel* model = new QStandardItemModel;
	model->setHorizontalHeaderItem(0, new QStandardItem("用户名"));
	model->setHorizontalHeaderItem(1, new QStandardItem("得分"));
	model->setHorizontalHeaderItem(2, new QStandardItem("用时"));
	model->setRowCount(userList.size());

	for (int i = 0; i < userList.size(); i++)
	{
		model->setItem(i, 0, new QStandardItem(userList[i].getName()));
		model->setItem(i, 1, new QStandardItem(QString::number(userList[i].getScore())));
		model->setItem(i, 2, new QStandardItem(QString::number(userList[i].getTime())));
	}
	viewRank->setModel(model);
	viewRank->show();
}

ScoreWindow::~ScoreWindow()
{
	disconnect(clientSocket, &QTcpSocket::disconnected, 0, 0);
	delete clientSocket;
	delete viewRank;
}

void ScoreWindow::getInfo()//接收服务器的响应
{
	QString msg = QString::fromUtf8(clientSocket->readAll());
	qDebug() << msg << endl;
	if (msg=="error")
	{
		QMessageBox::critical(NULL, "ERROR", "向服务器传输的消息类型错误");
	}
	else if (msg=="Database Error")
	{
		QMessageBox::critical(NULL, "ERROR", "服务器数据库配置错误");
	}
	else
	{
		QStringList list = msg.split(" ");

		for (int i = 0; i < list.size() - 1; i++)
		{
			qDebug() << list[i] << endl;
		}
		//将服务器响应信息存入用户信息列表
		for (int i = 0; i < list.size() - 1; i += 3)
		{
			QString uname = list[i], uscore = list[i + 1], utime = list[i + 2];
			userList.push_back(User(uname, uscore.toInt(), utime.toInt()));
		}
		//调试显示
		for (int i = 0; i < userList.size(); i++)
		{
			qDebug() << userList[i].getName() << userList[i].getScore() << userList[i].getTime() << endl;
		}
		emit signalDataReady();
	}
}

void ScoreWindow::sendRequest(int time, QString name)//发送连接请求
{
	//服务器未开启
	if (clientSocket->state()!= QAbstractSocket::ConnectedState)
	{
		QMessageBox::critical(NULL, "ERROR", "连接服务器失败");
	}
	else
	{
		//消息格式
		//update lxx 10 50 select
		QString text = "update " + name + " " + QString::number(10) + " " + QString::number(time) + " select";
		//向服务端发送数据
		qDebug() << text << endl;
		QByteArray bArray = text.toUtf8();
		clientSocket->write(bArray);
		//连接信号与槽以响应服务端来信
		connect(clientSocket, &QTcpSocket::readyRead, this, &ScoreWindow::getInfo);
	}
}