#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setGeometry(250, 100, 1400, 800);
	setWindowTitle("空当接龙");
	scene = new QGraphicsScene(this);
	scene->setSceneRect(-200, -200, this->geometry().width() - 20, this->geometry().height() - 20);
	scene->setBackgroundBrush(QPixmap(":/pic/Resources/background.JPG"));
	isClose = false;
	contrl = new Controller(this);
	contrl->initialCardHeap();
	contrl->shuffleCardHeap();
	contrl->initialBuffer();
	contrl->initialDest();
	contrl->addToList();
	contrl->initialCardRec();
	contrl->addToScene(scene);
	
	view = new QGraphicsView(this);
	view->setScene(scene);
	view->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
	view->show();

	retractionButton = new QPushButton(this);
	retractionButton->move(600, 600);
	retractionButton->setText("撤销");

	displaytime = new QLabel(this);
	displaytime->setGeometry(30, -70, 200, 300);
	displaytime->setStyleSheet("color:#AAAAAA;");

	connect(retractionButton, &QPushButton::clicked, contrl, &Controller::slotClickRetractionButton);
	connect(contrl, &Controller::signalPassed, this, &MainWindow::slotPassed);
	connect(contrl, &Controller::signalTimeout, this, &MainWindow::showtime);

	//这段代码仅用于测试
	//entryName = new EntryNameWindow(720);
	//connect(entryName, &EntryNameWindow::signalNextGame, this, &MainWindow::slotNewGame);
	//entryName->setWindowTitle("胜利");
	//entryName->show();
	//isClose = true;
}

MainWindow::~MainWindow()
{
	delete view;
	delete scene;
	delete retractionButton; 
	delete contrl;
	if (isClose)
	{
		delete entryName;
	}
}

void MainWindow::slotPassed()
{
	int time = timeNowMin * 60 + timeNowSec;
	EntryNameWindow* entryName = new EntryNameWindow(time);
	entryName->setWindowTitle("胜利");
	entryName->show();
	isClose = true;
}

void MainWindow::showtime() {
	timeNowSec++;
	if (timeNowSec == 60) {
		timeNowSec++;
		timeNowSec = 0;
	}
	QString show = "用时 " + QString::number(timeNowMin) + " min " + QString::number(timeNowSec) + " s";
	displaytime->setText(show);
}

void MainWindow::slotNewGame()
{
	disconnect(contrl, &Controller::signalTimeout, 0, 0);
	delete scene;
	delete contrl;

	scene = new QGraphicsScene(this);
	scene->setSceneRect(-200, -200, this->geometry().width() - 20, this->geometry().height() - 20);
	scene->setBackgroundBrush(QPixmap(":/pic/Resources/background.JPG"));
	isClose = false;

	contrl = new Controller(this);
	contrl->initialCardHeap();
	contrl->shuffleCardHeap();
	contrl->initialBuffer();
	contrl->initialDest();
	contrl->addToList();
	contrl->initialCardRec();
	contrl->addToScene(scene);

	view->setScene(scene);
	view->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
	view->show();

	timeNowSec = 0;
	timeNowMin = 0;

	connect(retractionButton, &QPushButton::clicked, contrl, &Controller::slotClickRetractionButton);
	connect(contrl, &Controller::signalPassed, this, &MainWindow::slotPassed);
	connect(contrl, &Controller::signalTimeout, this, &MainWindow::showtime);
}