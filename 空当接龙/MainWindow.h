#pragma once
#ifdef  _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "ui_MainWindow.h"
#include "Card.h"
#include "Controller.h"
#include "ScoreWindow.h"
#include "EntryNameWindow.h"
#include <QGraphicsView>
#include <QPainter>
#include <QPushButton>
#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);

	~MainWindow();

public slots:
	void slotPassed();

	void showtime();

	void slotNewGame();

private:
	Ui::MainWindowClass ui;
	QGraphicsView* view;
	QGraphicsScene* scene;
	Controller* contrl;
	EntryNameWindow* entryName;
	QPushButton* retractionButton;
	bool isClose;
	QLabel* displaytime;
	int timeNowSec = 0;
	int timeNowMin = 0;
};
