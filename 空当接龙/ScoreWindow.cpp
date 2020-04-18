#include "ScoreWindow.h"

ScoreWindow::ScoreWindow(int score /* = 0 */, int time /* = 0 */, QString name /* = "" */, QWidget* parent /* = Q_NULLPTR */)
	: QWidget(parent)
{
	ui.setupUi(this);
	setGeometry(300, 100, 762, 609);

	QPalette palColor;
	palColor.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/Resources/scoreWindow.png")));
	setPalette(palColor);//���õ�ɫ

	QString IPAddr = readIPAddr();//IP��ַ

	clientSocket = new QTcpSocket(this);//����TCP����
	clientSocket->connectToHost(IPAddr, 9999);

	viewRank = new QTableView(this);//���а�
	viewRank->setGeometry(150, 225, 400, 350);
	viewRank->setStyleSheet("background-color:#FFFFFF;");

	updateGame = new QPushButton(this);//��һ�ְ���
	updateGame->setGeometry(580, 480, 150, 40);
	updateGame->setText("��һ��");
	updateGame->setStyleSheet("color:#888888;");
	connect(updateGame, &QPushButton::clicked, this, &ScoreWindow::slotNextGameButton);

	//setAttribute(Qt::WA_DeleteOnClose, 1);

	testConnection();
	//������ʾ
	connect(clientSocket, &QTcpSocket::disconnected, []() {
		QMessageBox::critical(NULL, "ERROR", "�����ѶϿ�");
		});
	//�����������¼�
	connect(this, &ScoreWindow::signalTestConn, [=](bool isConnecting)
		{
			if (isConnecting)
			{
				//���յ���Ϣ�󣬽����ݴ��뱾�����ݽṹ�û���Ϣ������
				connect(clientSocket, &QTcpSocket::readyRead, this, &ScoreWindow::getInfo);
				//�û���Ϣ����׼���ú󣬿�ʼ��ʾ
				connect(this, &ScoreWindow::signalDataReady, this, &ScoreWindow::showData);
				//�����������������Ϣ
				sendRequest(time, name);
			}
			else
			{
				QMessageBox::critical(NULL, "ERROR", "����δ����");
			}
		});
}

void ScoreWindow::slotNextGameButton()//��һ��
{
	emit signalNextGame();
	this->close();
}

QString ScoreWindow::readIPAddr()//���ļ��Ӷ���ȡIP��ַ
{
	QFile f(":/txt/IPAddr");
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))//��ָ���ļ�
	{
		QMessageBox::about(NULL, "�ļ�", "IPAddr.txt��ʧ��");
		return "";
	}
	QTextStream txtInput(&f);
	QString IPAddr = txtInput.readLine();  //��ȡ����
	f.close();
	return IPAddr;
}

void ScoreWindow::lookUpHost(QHostInfo info)//lookupHost�Ĵ����������亯��ָ�봫��
{
	if (info.error()== QHostInfo::NoError)
	{
		qDebug() << "��������" << endl;
		emit signalTestConn(true);
	}
	else
	{
		qDebug() << "�����쳣" << endl;
		emit signalTestConn(false);
	}
}

void ScoreWindow::testConnection()//�������״��
{
	QHostInfo::lookupHost("www.baidu.com", this, SLOT(lookUpHost(QHostInfo)));
}


void ScoreWindow::showData()//��ʾ���а���Ϣ
{
	std::sort(userList.begin(), userList.end(), [](User lhs, User rhs) { return lhs.getTime() < rhs.getTime(); });
	QStandardItemModel* model = new QStandardItemModel;
	model->setHorizontalHeaderItem(0, new QStandardItem("�û���"));
	model->setHorizontalHeaderItem(1, new QStandardItem("�÷�"));
	model->setHorizontalHeaderItem(2, new QStandardItem("��ʱ"));
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

void ScoreWindow::getInfo()//���շ���������Ӧ
{
	QString msg = QString::fromUtf8(clientSocket->readAll());
	qDebug() << msg << endl;
	if (msg=="error")
	{
		QMessageBox::critical(NULL, "ERROR", "��������������Ϣ���ʹ���");
	}
	else if (msg=="Database Error")
	{
		QMessageBox::critical(NULL, "ERROR", "���������ݿ����ô���");
	}
	else
	{
		QStringList list = msg.split(" ");

		for (int i = 0; i < list.size() - 1; i++)
		{
			qDebug() << list[i] << endl;
		}
		//����������Ӧ��Ϣ�����û���Ϣ�б�
		for (int i = 0; i < list.size() - 1; i += 3)
		{
			QString uname = list[i], uscore = list[i + 1], utime = list[i + 2];
			userList.push_back(User(uname, uscore.toInt(), utime.toInt()));
		}
		//������ʾ
		for (int i = 0; i < userList.size(); i++)
		{
			qDebug() << userList[i].getName() << userList[i].getScore() << userList[i].getTime() << endl;
		}
		emit signalDataReady();
	}
}

void ScoreWindow::sendRequest(int time, QString name)//������������
{
	//������δ����
	if (clientSocket->state()!= QAbstractSocket::ConnectedState)
	{
		QMessageBox::critical(NULL, "ERROR", "���ӷ�����ʧ��");
	}
	else
	{
		//��Ϣ��ʽ
		//update lxx 10 50 select
		QString text = "update " + name + " " + QString::number(10) + " " + QString::number(time) + " select";
		//�����˷�������
		qDebug() << text << endl;
		QByteArray bArray = text.toUtf8();
		clientSocket->write(bArray);
		//�����ź��������Ӧ���������
		connect(clientSocket, &QTcpSocket::readyRead, this, &ScoreWindow::getInfo);
	}
}