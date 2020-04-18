#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <qglobal.h>
#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThreadPool>


class MulServer : public QTcpServer
{
	Q_OBJECT

public:
	MulServer(QObject* parent);
	~MulServer();

	void incomingConnection(qintptr handle) override;
	void replyToClient(QTcpSocket* socket);
	void showConnection(QTcpSocket* socket);
	void writeToSocket(qintptr handle, QString str);
	
signals:
	void readyToReply(QTcpSocket* socket);
	void serverReceived(QTcpSocket* socket);
	
private:
	QMap<qintptr, QTcpSocket*> clientList;
};
