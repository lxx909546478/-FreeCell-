#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include<QString>

class User
{
public:
	User();

	User(QString name, int score, int time);

	~User();

	QString getName();

	int getScore();

	int getTime();

private:
	QString name;
	int score;
	int time;
};


