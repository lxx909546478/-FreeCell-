#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QByteArray>
#include <QObject>
#include <QRunnable>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QTcpSocket>


class DBThread : public QObject, public QRunnable
{
	Q_OBJECT

public:
	DBThread(qintptr handle, QObject* parent = Q_NULLPTR);
	DBThread(qintptr handle, QString info, QObject* parent = Q_NULLPTR);
	~DBThread();
	bool connectToSql(const QString dbName);
	QString getInfo();

signals:
	void finishDB(qintptr handle, QString str);
	void finished();

protected:
	void run()override;

private:
	qintptr handle;
	QString updateInfo;
	QSqlDatabase myDB;
};
