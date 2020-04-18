#include <QtCore/QCoreApplication>
#include "MulServer.h"
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	MulServer* server = new MulServer(&a);
	return a.exec();
}
