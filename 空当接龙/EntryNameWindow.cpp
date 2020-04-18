#include "EntryNameWindow.h"

EntryNameWindow::EntryNameWindow(int time, QWidget* parent /* = nullptr */)
	:QWidget(parent)
{
	ui.setupUi(this);//设置界面属性
	setFixedSize(511, 415);

	QPalette pal;
	pal.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/Resources/timg.png")));
	setPalette(pal);//设置界面背景颜色

	labelTip = new QLabel(this);
    labelTip->setText("请输入您的名字");
	labelTip->setGeometry(110, 160, 280, 100);
	labelTip->setStyleSheet("color:#AAAAAA;");//设置提示信息

	entryName = new QLineEdit(this);
	entryName->setGeometry(120, 240, 250, 30);
	entryName->setStyleSheet("background-color:#AAAAAA;");//设置输入框属性

	buttonSure = new QPushButton(this);
	buttonSure->setGeometry(210, 275, 80, 40);
	buttonSure->setText("确认");
	buttonSure->setStyleSheet("color:#AAAAAA;");//设置确认按钮属性
	connect(buttonSure, &QPushButton::clicked, this, &EntryNameWindow::on_buttonSure_clicked);
	
	timeofUser = time;//用户所用时间初始化
	//setAttribute(Qt::WA_DeleteOnClose, 1);
}

EntryNameWindow::~EntryNameWindow()
{
	if (isOpenScore)
	{
		delete scoreRank;
	}
}

void EntryNameWindow::on_buttonSure_clicked() //确认按钮槽函数
{
	QString NameofUser = entryName->text();
	qDebug() << NameofUser << endl;

	scoreRank = new ScoreWindow(10, timeofUser, NameofUser);//在构造函数中将name、timer传给ScoreWindow
	scoreRank->setWindowTitle("排行榜");
	
	connect(scoreRank, &ScoreWindow::signalNextGame, this, &EntryNameWindow::slotNextGame);
	scoreRank->show();
	
	isOpenScore = true;
	setVisible(false);
}

void EntryNameWindow::slotNextGame()
{
	emit signalNextGame();
	this->close();
}



