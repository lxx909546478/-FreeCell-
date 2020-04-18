#include "EntryNameWindow.h"

EntryNameWindow::EntryNameWindow(int time, QWidget* parent /* = nullptr */)
	:QWidget(parent)
{
	ui.setupUi(this);//���ý�������
	setFixedSize(511, 415);

	QPalette pal;
	pal.setBrush(QPalette::Background, QBrush(QPixmap(":/pic/Resources/timg.png")));
	setPalette(pal);//���ý��汳����ɫ

	labelTip = new QLabel(this);
    labelTip->setText("��������������");
	labelTip->setGeometry(110, 160, 280, 100);
	labelTip->setStyleSheet("color:#AAAAAA;");//������ʾ��Ϣ

	entryName = new QLineEdit(this);
	entryName->setGeometry(120, 240, 250, 30);
	entryName->setStyleSheet("background-color:#AAAAAA;");//�������������

	buttonSure = new QPushButton(this);
	buttonSure->setGeometry(210, 275, 80, 40);
	buttonSure->setText("ȷ��");
	buttonSure->setStyleSheet("color:#AAAAAA;");//����ȷ�ϰ�ť����
	connect(buttonSure, &QPushButton::clicked, this, &EntryNameWindow::on_buttonSure_clicked);
	
	timeofUser = time;//�û�����ʱ���ʼ��
	//setAttribute(Qt::WA_DeleteOnClose, 1);
}

EntryNameWindow::~EntryNameWindow()
{
	if (isOpenScore)
	{
		delete scoreRank;
	}
}

void EntryNameWindow::on_buttonSure_clicked() //ȷ�ϰ�ť�ۺ���
{
	QString NameofUser = entryName->text();
	qDebug() << NameofUser << endl;

	scoreRank = new ScoreWindow(10, timeofUser, NameofUser);//�ڹ��캯���н�name��timer����ScoreWindow
	scoreRank->setWindowTitle("���а�");
	
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



