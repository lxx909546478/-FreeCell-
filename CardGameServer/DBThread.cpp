#include "DBThread.h"

DBThread::DBThread(qintptr handle, QObject* parent /* = Q_NULLPTR */)
	: QObject(parent)
{
	this->handle = handle;
}

DBThread::DBThread(qintptr handle, QString info, QObject* parent /* = Q_NULLPTR */)
{
	this->handle = handle;
	this->updateInfo = info;
}

DBThread::~DBThread()
{
	qDebug() << "线程已退出" << endl;
}

void DBThread::run()
{
	QString replyMsg = getInfo();
	emit finishDB(handle, replyMsg);
}

bool DBThread::connectToSql(const QString dbName)
{
	if (QSqlDatabase::contains("qt_sql_default_connection"))
		myDB = QSqlDatabase::database("qt_sql_default_connection");
	else
		myDB = QSqlDatabase::addDatabase("QMYSQL");

	myDB.setHostName("127.0.0.1");
	myDB.setDatabaseName(dbName);
	myDB.setUserName("root");
	myDB.setPassword("");

	if (!myDB.open()) {
		qDebug() << (0, QObject::tr("Database Error"), myDB.lastError().text());
		return false;
	}
	return true;
}

QString DBThread::getInfo()
{
	QString answer = "";
	if (connectToSql("lxx"))
	{
		QSqlQuery query;
		QStringList strList = updateInfo.split(" ");
		QString name = strList[1], score = strList[2], time = strList[3];
		QString execStr = "";
		query.exec("select* from card_game_score where name = '" + name + "'");
		if (!query.next())
		{
			execStr = "insert into card_game_score(name, score, time) "\
				"values('" + name + "', " + score + ", " + time + " )";
		}
		else
		{
			if (query.value("time").toInt() < time.toInt())
			{
				time = query.value("time").toString();
			}
			execStr = "update card_game_score "\
				"set score = score + " + strList[2] + ", time = " + time +
				" where name = '" + strList[1] + "'";
		}

		qDebug() << execStr << endl;
		query.exec(execStr);

		query.exec("select* from card_game_score");
		while (query.next())
		{
			answer += (query.value("name").toString() + " "
				+ query.value("score").toString() + " "
				+ query.value("time").toString() + " ");
		}
		myDB.close();
	}
	else
	{
		answer = "Database Error";
	}
	return answer;
}