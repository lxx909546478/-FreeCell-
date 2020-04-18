#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <iostream>
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
