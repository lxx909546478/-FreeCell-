#include "MulServer.h"
#include "DBThread.h"
#include <QThreadPool>

MulServer::MulServer(QObject* parent)
	: QTcpServer(parent)
{
	listen(QHostAddress::Any, 9999);
	setMaxPendingConnections(1000);
	connect(this, &MulServer::serverReceived, this, &MulServer::showConnection);
	connect(this, &MulServer::readyToReply, this, &MulServer::replyToClient);
	
}

MulServer::~MulServer()
{
	QMap<qintptr, QTcpSocket*>::iterator iter;
	for (iter=clientList.begin();iter!=clientList.end();iter++)
	{
		iter.value()->disconnectFromHost();
		delete iter.value();
	}
}

void MulServer::incomingConnection(qintptr handle)
{
	QTcpSocket* clientSocket = new QTcpSocket;
	clientSocket->setSocketDescriptor(handle);
	if (clientList.size() > maxPendingConnections())
	{
		qDebug() << maxPendingConnections() << endl;
		clientSocket->disconnectFromHost();
		return;
	}
	clientList.insert(handle, clientSocket);
	qDebug() << handle << "connected" << clientList.value(handle)->socketDescriptor();
	emit serverReceived(clientSocket);

	connect(clientSocket, &QTcpSocket::disconnected, [=]() {
		clientSocket->deleteLater();
		clientList.remove(handle);
		qDebug() << handle << "disconnect" << endl;
		});
	
	connect(clientSocket, &QTcpSocket::readyRead, [=]() {
		emit readyToReply(clientSocket);
		});
}

void MulServer::replyToClient(QTcpSocket* socket)
{
	QByteArray bArray = socket->readAll();
	QString msg = QString::fromUtf8(bArray);
	qDebug() << socket->peerAddress().toString() << socket->peerPort() << ":" << msg << endl;
	DBThread* task = NULL;
	
	if (msg.size()<6)
	{
		qDebug() << "wrong request" << endl;
		writeToSocket(socket->socketDescriptor(), "error");
	}
	else if (msg.mid(0,6)=="update")
	{
		task= new DBThread(socket->socketDescriptor(), msg);
		connect(task, &DBThread::finishDB, this, &MulServer::writeToSocket, Qt::BlockingQueuedConnection);
		//connect(task, &DBThread::finished, task, &DBThread::deleteLater);
	}
	else
	{
		qDebug() << "wrong request" << endl;
		writeToSocket(socket->socketDescriptor(), "error");
	}
	
	QThreadPool::globalInstance()->start(task);
}

void MulServer::writeToSocket(qintptr handle, QString str)
{
	qDebug() << str << endl;
	clientList.value(handle)->write(str.toUtf8());
}

void MulServer::showConnection(QTcpSocket* socket)
{
	qDebug() << socket->peerAddress().toString() << socket->peerPort();
}





