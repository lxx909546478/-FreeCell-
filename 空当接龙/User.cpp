#include "User.h"

User::User()
{
}

User::~User()
{
}

User::User(QString name, int score, int time)
{
	this->name = name;
	this->score = score;
	this->time = time;
}

QString User::getName() 
{
	return name;
}

int User::getScore()
{
	return score;
}

int User::getTime()
{
	return time;
}

